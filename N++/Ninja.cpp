#include "Ninja.h"



Ninja::Ninja() :
	Ninja({ 0, 0 })
{
}

Ninja::Ninja(Vector2 start) :
	velocity(0, 0),
	
	groundNormal(nullptr),
	wallNormal(nullptr),

	movement(NINJA_STILL),
	jump(false),
	jumping(false),
	jumpDuration(0),

	currentPose(POSE_NONE)
{
	hull = ConvexHull({
		{ 0, 0 },
		{ 0.5, 0 },
		{ 0.5, 0.9 },
		{ 0, 0.9 },
		});

	hull.translate(start);

	skeleton = this->getSkeleton();
}

Ninja::~Ninja()
{
	delete this->groundNormal;
	delete this->wallNormal;
}

void Ninja::move(NinjaMovement movement)
{
	switch (movement)
	{
	case NINJA_LEFT: this->movement = (this->movement == NINJA_RIGHT ? NINJA_STILL : NINJA_LEFT); break;
	case NINJA_RIGHT: this->movement = (this->movement == NINJA_LEFT ? NINJA_STILL : NINJA_RIGHT); break;

	case NINJA_JUMP: this->jump = true; break;
	case NINJA_CANCEL_JUMP: this->jump = false; this->jumping = false; break;

	case NINJA_DROP: delete this->wallNormal; this->wallNormal = nullptr; break;

	default: break;
	}
}

void Ninja::update(double deltaTime, const std::vector<const Collider*>& colliders)
{
	Vector2 moveDirection = (this->groundNormal ? this->groundNormal->perpCCW() : Vector2(1.0, 0.0));
	switch (this->movement)
	{
	case NINJA_LEFT: moveDirection *= -1; break;
	case NINJA_RIGHT: moveDirection *= 1; break;
	default: moveDirection *= 0;
		break;
	}

	// Beräkna rörelsens påverkan
	if (!this->wallNormal) {
		velocity += moveDirection * deltaTime * MOVE_SPEED;
	}

	// Beräkna hoppandets påverkan
	if (this->jump && this->groundNormal) {
		this->jump = false;
		this->jumping = true;
		this->jumpDuration = 0;

		velocity += *this->groundNormal * JUMP_FORCE;
		//velocity.x += this->groundNormal->x * JUMP_FORCE;
	} else if (this->jump && this->wallNormal) {
		this->jump = false;
		this->jumping = true;
		this->jumpDuration = 0;

		velocity += (*this->wallNormal + Vector2(0, -1)).normal() * JUMP_FORCE;
	}

	// Beräkna gravitationens påverkan
	if (velocity.y > 0.0) {
		velocity.y += 2 * GRAVITY * deltaTime;
	}
	else {
		velocity.y += GRAVITY * deltaTime;
	}
	
	// Beräkna luftmotståndets påverkan
	velocity -= DRAG * velocity * deltaTime;

	// Beräkna motståndet när ninjan har en pose
	if(this->isPosing()) {
		velocity.x -= 10 * velocity.x * deltaTime;
	}
	
	// Beräkna friktionens påverkan om ninjan nuddar väggen
	if (this->wallNormal && this->velocity.y > 0) {
		velocity -= WALL_FRICTION * velocity * deltaTime;
	}

	// Förflytta ninjan
	hull.translate(velocity * deltaTime);


	// Kolla efter och åtgärda kollisioner
	this->resolveCollisions(colliders);

	this->updateSkeleton(deltaTime);

	// Återställ rörelse
	this->movement = NINJA_STILL;
}

void Ninja::render(Renderer & renderer)
{
#if 0
	// Rita kollisionsskalet
	renderer.setLineStyle(LINE_NONE);

	if (this->wallNormal) {
		renderer.setFillColor(200, 50, 200);
	}
	else {
		renderer.setFillColor(50, 200, 200);
	}

	this->hull.fill(renderer);
#endif

	renderer.setLineStyle(LINE_SOLID);
	this->skeleton.draw(renderer);
}

const ConvexHull& Ninja::getConvexHull() const
{
	return this->hull;
}

void Ninja::pose(NinjaPose ninjaPose)
{
	this->currentPose = ninjaPose;
}

bool Ninja::isPosing()
{
	return bool(this->currentPose);
}

void Ninja::resolveCollisions(const std::vector<const Collider*>& colliders)
{
	// Antalet kollisionsiterationer hittils
	int iterations = 0;

	// Den sista förflyttningen ur en kollision
	Vector2* finalNormal = nullptr;

	// Ninjan måste kollidera med marken för att nudda den
	delete this->groundNormal; this->groundNormal = nullptr;

	while (true) {
		if (Vector2* firstOverlap = findFirstOverlap(this->hull, colliders)) {
			this->handleOverlap(*firstOverlap);

			// Spara detta som den senaste kollisionen
			delete finalNormal;
			finalNormal = new Vector2(-firstOverlap->normal());

			delete firstOverlap;
		}
		else {
			break;
		}

		// Om spelaren hamnar i en "kollisionsloop" (hoppar innuti två objekt) sluta efter 10 iterationer
		iterations++;
		if (iterations > 10) {
			break;
		}
	}

	// Om det var en kollision, anpassa hastigheten
	if (finalNormal) {
		this->handleCollision(*finalNormal);
		delete finalNormal;
	}
	
	// Kolla ifall väggen "finns kvar"/ninjan har ramlat av väggen
	if (this->wallNormal) {
		Vector2 nudge = *this->wallNormal * -0.0005;
		this->hull.translate(nudge);
		if (Vector2* firstOverlap = findFirstOverlap(this->hull, colliders)) {
			delete firstOverlap;
		}
		else {
			delete this->wallNormal; this->wallNormal = nullptr;
		}

		this->hull.translate(-nudge);
	}
}

void Ninja::handleOverlap(Vector2 overlap)
{
	// Förflytta spelaren ur kollisionen
	hull.translate(-overlap);

	// Beräkna det kolliderade planets normal
	Vector2 normal = -overlap.normal();

	// Kolla om spelaren nuddar marken (ett plan som lutar mindre än 60 grader)
	if (normal.dot({ 0.0, -1.0 }) > cos(PI * 60.0 / 180.0)) {
		this->groundNormal = new Vector2(normal);
		delete this->wallNormal; this->wallNormal = nullptr;
	}
	// Kolla om spelaren nuddar väggen (ett plan som är lodrätt)
	else if (!this->groundNormal && (normal.dot({ 1, 0 }) > 0.99 || normal.dot({ -1, 0 }) > 0.99)) {
		delete this->wallNormal;
		this->wallNormal = new Vector2(normal);
	}
}

void Ninja::handleCollision(Vector2 normal)
{
	// Hantera endast om kollisionens normal är motsatt färdriktningen
	if (normal.dot(this->velocity) < 0) {
		Vector2 plane = { normal.y, -normal.x };

		double dot = plane.dot(velocity.normal());

		if ((velocity.x > 0 && movement == NINJA_RIGHT || velocity.x < 0 && movement == NINJA_LEFT) &&
			normal.y <= 0 && plane.y * dot <= 0)
		{
			velocity = plane * velocity.length() * (dot < 0 ? -1 : dot > 0 ? 1 : 0) * pow(abs(dot), 1.0 / 4);
		} else
		{
			velocity = plane * plane.dot(velocity);
		}
	}
}

void Ninja::updateSkeleton(double deltaTime)
{
	Skeleton target = this->getSkeleton();

	// Hur snabbt ninjan anpassar sig till sin nya form
	const double factor = lerp(normalize(abs(this->velocity.x), 0.0, 4.5), 8, 15);
	
	skeleton.shoulder -= (skeleton.shoulder - target.shoulder) * deltaTime * factor;
	skeleton.hip -= (skeleton.hip - target.hip) * deltaTime * factor;

	for (int i = 0; i < 2; i++)
	{
		skeleton.head[i] -= (skeleton.head[i] - target.head[i]) * deltaTime * factor;

		skeleton.elbows[i] -= (skeleton.elbows[i] - target.elbows[i]) * deltaTime * factor;
		skeleton.hands[i] -= (skeleton.hands[i] - target.hands[i]) * deltaTime * factor;
		skeleton.knees[i] -= (skeleton.knees[i] - target.knees[i]) * deltaTime * factor;
		skeleton.feet[i] -= (skeleton.feet[i] - target.feet[i]) * deltaTime * factor;
	}

	// Flytta alla delar så att höften hålls i mitten av kollisionsskalet
	Vector2 delta = this->hull.average() - skeleton.hip;

	skeleton.shoulder += delta;
	skeleton.hip += delta;

	for (int i = 0; i < 2; i++)
	{
		skeleton.head[i] += delta;

		skeleton.elbows[i] += delta;
		skeleton.hands[i] += delta;
		skeleton.knees[i] += delta;
		skeleton.feet[i] += delta;
	}
}


Ninja::Skeleton Ninja::getSkeleton()
{
	BoundingBox bounds = this->hull.getBoundingBox();

	SkeletonData data;
	
	data.bounds = bounds;

	data.width = bounds.right - bounds.left;
	data.height = bounds.bottom - bounds.top;

	data.mid = Vector2((bounds.left + bounds.right) / 2, (bounds.top + bounds.bottom) / 2);

	data.backLength = data.height / 3;
	data.armLength = data.height / 5;
	data.legLength = data.height / 4;
	
	data.groundNormal = this->groundNormal;
	data.wallNormal = this->wallNormal;

	data.velocity = this->velocity;
	data.pose = this->currentPose;

	return Skeleton(data);
}


Ninja::Skeleton::Skeleton(SkeletonData data)
{
	this->shoulder = { data.mid.x, data.bounds.top };
	this->hip = { data.mid.x, data.mid.y };

	// Står i en pose
	if (data.pose != POSE_NONE) {
		switch (data.pose)
		{
		case POSE_VICTORY: this->victory(data); break;
		}
	}

	// Springer/står stilla
	else if (data.groundNormal) {
		this->stand(data);
	}
	
	// Klättrar på en vägg
	else if (data.wallNormal) {
		this->climb(data);
	}

	// I luften: hoppar eller faller
	else {
		this->fall(data);
	}
}

void Ninja::Skeleton::draw(Renderer & renderer)
{
	renderer.drawLine(this->head[0], this->head[1]);
	renderer.drawLine(this->shoulder, this->hip);

	for (int i = 0; i < 2; i++)
	{
		renderer.drawLine(this->shoulder, this->elbows[i]);
		renderer.drawLine(this->elbows[i], this->hands[i]);

		renderer.drawLine(this->hip, this->knees[i]);
		renderer.drawLine(this->knees[i], this->feet[i]);
	}
}

void mirror(double* a, double pivot) {
	*a = pivot + (pivot - *a);
}

void Ninja::Skeleton::mirrorX(double x)
{
	mirror(&shoulder.x, x);
	mirror(&hip.x, x);

	for (int i = 0; i < 2; i++)
	{
		mirror(&head[i].x, x);

		mirror(&elbows[i].x, x);
		mirror(&hands[i].x, x);
		mirror(&knees[i].x, x);
		mirror(&feet[i].x, x);
	}
}

void Ninja::Skeleton::createHead(SkeletonData data)
{
	double vy = normalize(-data.velocity.y - 1.0, -2.0, 2.0);

	double angle = lerp(vy, -25, 35);
	this->head[0] = this->shoulder - data.width / 7 * Vector2::rotated(angle) - Vector2(0, data.height / 10);
	this->head[1] = this->shoulder + data.width / 5 * Vector2::rotated(angle) - Vector2(0, data.height / 10);
}

void Ninja::Skeleton::stand(SkeletonData data)
{
	double vx = normalize(abs(data.velocity.x), 0, 2.5);

	double stepLength = data.width * 3;
	double p = 2 * fmod(data.mid.x, stepLength * 2) / (stepLength * 2);

	bool swapped = false;
	if (p > 1.0) {
		swapped = true;
		p -= 1;
	}

	this->shoulder = this->hip + data.backLength * Vector2::rotated(80);


	double armAngles[2] = {
		lerp(p, 330.0, 200.0),
		lerp(p, 200.0, 330.0),
	};


	this->elbows[0] = this->shoulder + data.armLength * Vector2::rotated(armAngles[0]);
	this->elbows[1] = this->shoulder + data.armLength * Vector2::rotated(armAngles[1]);

	this->hands[0] = this->elbows[0] + data.armLength * Vector2::rotated(armAngles[0] + 70);
	this->hands[1] = this->elbows[1] + data.armLength * Vector2::rotated(armAngles[1] + 70);


	double legMax = 250 - 30 * vx; // 270 - 50
	double legMin = 290 + 30 * vx; // 270 + 50

	double legAngles[2] = {
		lerp(p, legMin, legMax),
		lerp(p, legMax, legMin),
	};

	this->knees[0] = this->hip + data.legLength * Vector2::rotated(legAngles[0]);
	this->knees[1] = this->hip + data.legLength * Vector2::rotated(legAngles[1]);


	double feetMax = 100 * vx;

	this->feet[0] = this->knees[0] + data.legLength * Vector2::rotated(legAngles[0] - lerp(p, 0.0, feetMax));
	this->feet[1] = this->knees[1] + data.legLength * Vector2::rotated(legAngles[1] - lerp(p*p, feetMax, 0.0));

	if (swapped) {
		std::swap(this->elbows[0], this->elbows[1]);
		std::swap(this->hands[0], this->hands[1]);
		std::swap(this->knees[0], this->knees[1]);
		std::swap(this->feet[0], this->feet[1]);
	}

	this->createHead(data);

	if (data.velocity.x < 0) {
		this->mirrorX(data.mid.x);
	}
}

void Ninja::Skeleton::climb(SkeletonData data)
{
	this->shoulder = this->hip + data.backLength * Vector2::rotated(75);

	this->elbows[0] = this->shoulder + data.armLength * Vector2::rotated(160);
	this->elbows[1] = this->shoulder + data.armLength * Vector2::rotated(210);

	this->hands[0] = this->elbows[0] + data.armLength * Vector2::rotated(140);
	this->hands[1] = this->elbows[1] + data.armLength * Vector2::rotated(140);
	
	this->knees[0] = this->hip + data.legLength * Vector2::rotated(130);
	this->knees[1] = this->hip + data.legLength * Vector2::rotated(240);

	this->feet[0] = this->knees[0] + data.legLength * Vector2::rotated(240);
	this->feet[1] = this->knees[1] + data.legLength * Vector2::rotated(240);

	this->createHead(data);

	if (data.wallNormal->x < 0) {
		this->mirrorX(data.mid.x);
	}
}

void Ninja::Skeleton::fall(SkeletonData data)
{
	double vy = normalize(-data.velocity.y, -1, 1.5);

	this->shoulder = this->hip + data.backLength * Vector2::rotated(lerp(vy, 100, 70));

	this->elbows[0] = this->shoulder + data.armLength * Vector2::rotated(lerp(vy, 0, -60));
	this->elbows[1] = this->shoulder + data.armLength * Vector2::rotated(lerp(vy, 180, 210));

	this->hands[0] = this->elbows[0] + data.armLength * Vector2::rotated(lerp(vy, 30, 50));
	this->hands[1] = this->elbows[1] + data.armLength * Vector2::rotated(lerp(vy, 200, 220));


	this->knees[0] = this->hip + data.legLength * Vector2::rotated(lerp(vy, -60, 10));
	this->knees[1] = this->hip + data.legLength * Vector2::rotated(lerp(vy, 350, 240));

	this->feet[0] = this->knees[0] + data.legLength * Vector2::rotated(lerp(vy, 300, 240));
	this->feet[1] = this->knees[1] + data.legLength * Vector2::rotated(lerp(vy, 220, 240));

	this->createHead(data);

	if (data.velocity.x < 0) {
		this->mirrorX(data.mid.x);
	}
}

void Ninja::Skeleton::victory(SkeletonData data)
{
	this->shoulder = this->hip + data.backLength * Vector2::rotated(90);

	this->elbows[0] = this->shoulder + data.armLength * Vector2::rotated(90 + 70);
	this->elbows[1] = this->shoulder + data.armLength * Vector2::rotated(90 - 70);

	this->hands[0] = this->elbows[0] + data.armLength * Vector2::rotated(90 + 20);
	this->hands[1] = this->elbows[1] + data.armLength * Vector2::rotated(90 - 20);

	this->knees[0] = this->hip + data.legLength * Vector2::rotated(250);
	this->knees[1] = this->hip + data.legLength * Vector2::rotated(290);

	this->feet[0] = this->knees[0] + data.legLength * Vector2::rotated(260);
	this->feet[1] = this->knees[1] + data.legLength * Vector2::rotated(280);

	this->createHead(data);

	if (data.velocity.x > 0) {
		this->mirrorX(data.mid.x);
	}
}
