#pragma once

#include "ConvexHull.h"
#include "Collider.h"
#include "Interpolate.h"

// De olika sätt en Ninja kan röra sig på
enum NinjaMovement {
	NINJA_STILL,
	NINJA_LEFT,
	NINJA_RIGHT,

	NINJA_JUMP,
	NINJA_CANCEL_JUMP,

	NINJA_DROP
};


 // De olika sätt en ninja kan göra en pose
enum NinjaPose
{
	POSE_NONE,
	POSE_VICTORY
};


// Variabler för att justera parametrar för ninjans rörelse
const double MOVE_SPEED = 6.25;
const double GRAVITY = 4.2;

const Vector2 DRAG = {1.0, 0.5};

const double WALL_FRICTION = 6;

const double JUMP_FORCE = 6.2;
const double MAX_JUMP_DURATION = 1.0;


class Ninja
{
	// Ett konvext skal för kollision
	ConvexHull hull;


	struct SkeletonData
	{
		BoundingBox bounds;

		double width, height;
		double backLength, armLength, legLength;
		Vector2 mid;

		Vector2 velocity;

		Vector2* groundNormal, * wallNormal;

		NinjaPose pose;
	};

	// Positioner för en ninjas alla leder 
	struct Skeleton {
		Vector2 head[2];

		Vector2 shoulder;
		Vector2 elbows[2];
		Vector2 hands[2];

		Vector2 hip;

		Vector2 knees[2];
		Vector2 feet[2];


		Skeleton() = default;

		// Beräkna skelettets strutkur utifrån en bunt parametrar
		Skeleton(SkeletonData data);


		// Rita skelettet
		void draw(Renderer& renderer);

		// Spegla skelettet runt en punkt i x-led
		void mirrorX(double x);

	private:

		// Skapar skelettets huvud
		void createHead(SkeletonData data);


		// Skapar ett skelett som står stilla
		void stand(SkeletonData data);

		// Skapar ett skelett som klättrar
		void climb(SkeletonData data);

		// Skapar ett skelett som faller
		void fall(SkeletonData data);

		// Skapar ett skelett som står i en vinnarpose
		void victory(SkeletonData data);
	} skeleton;


	// Den nuvarande hastigheten och riktningen
	Vector2 velocity;

	
	// Lagrar markens normal relativt till spelaren, om ninjan nuddar marken
	Vector2* groundNormal;

	// Lagrar väggens normal relativt till spelaren, om ninjan nuddar väggen
	Vector2* wallNormal;


	// Åt vilket håll ninjan ska röra på sig
	NinjaMovement movement;

	// Borde ninjan hoppa?
	bool jump;

	// Håller ninjan på att hoppa?
	bool jumping;

	// Hur länge ninjan har hoppat
	double jumpDuration;


	// Den nuvarande pose ninjan har
	NinjaPose currentPose;


public:

	Ninja();
	Ninja(Vector2 start);

	~Ninja();


	// Anger att ninjan ska röra sig åt ett håll nästa uppdatering
	void move(NinjaMovement movement);

	// Uppdaterar ninjans position, hanterar kollision, etc
	void update(double deltaTime, const std::vector<const Collider*>& colliders);

	// Ritar ninjan till en ritare
	void render(Renderer& renderer);


	// Returnerar ninjans konvexa skal
	const ConvexHull& getConvexHull() const;


	// Anger att ninjan ska anta en pose
	void pose(NinjaPose ninjaPose);

	// Avgör om ninjan gör en pose
	bool isPosing();


private:

	// Förflytta ninjan ur kollisioner
	void resolveCollisions(const std::vector<const Collider*>& colliders);


	// Vid varje kollision, försöker förflytta ninjan ur kollisionen
	void handleOverlap(Vector2 overlap);

	// Anpassa hastigheten och färdriktningen efter en kollision
	void handleCollision(Vector2 normal);


	// Uppdatera ninjans leder beroende på dess rörelse
	void updateSkeleton(double deltaTime);

	// Returnera den nuvarande strukturen av ninjans leder
	// TODO: Dela upp i mindre delar
	Skeleton getSkeleton();
};

