#include "FunctionalTiles.h"

////////////////////////////////////////////////////

ExitTile::ExitTile(bool open) :
	open(open),
	openAmount(double(open)),
	time(0)
{
	this->setPosition({ 0 });
}

Tile* ExitTile::clone()
{
	return new ExitTile(*this);
}

void ExitTile::setPosition(Vector2i position)
{
	this->position = position;
}

void ExitTile::update(InteractionHandler* handler, double deltaTime)
{
	this->time += deltaTime;
	if(this->open) {
		this->openAmount = clamp(this->openAmount + deltaTime * 4, 0.0, 1.0);
	}
}

void ExitTile::render(Renderer& renderer)
{
	double openWidth = easeInOut(this->openAmount, 0.0, 1.0);

	Color background(50);

	renderer.setFillColor(background);
	renderer.fillRect(this->position.x + 0.5 - openWidth / 2, this->position.y, openWidth, 1);

	renderer.setFillColor(Color(255, 255, 0));
	this->renderHighlight(renderer, openWidth,
		Color(255, 0, 255),
		Color(background)
	);

	renderer.setLineStyle(LINE_SOLID);
	renderer.setColor(Color(0));
	renderer.setFillColor(150, 150, 150);


	this->renderDoorway(renderer, -openWidth / 2, -0.5);
	this->renderDoorway(renderer, openWidth / 2, 0.5);
}

Vector2* ExitTile::overlap(const ConvexHull& other) const
{
	Vector2 average = other.average();
	Vector2 center = Vector2(this->position) + Vector2(0.5, 0.5);

	Vector2 delta = center - average;
	double distance = delta.length();
	if (distance < 0.5)
	{
		return new Vector2(delta.normal() * distance);
	}

	return nullptr;
}

bool ExitTile::isPassable() const
{
	return true;
}

bool ExitTile::isDynamic() const {
	return true;
}

std::string ExitTile::getFormattedName() const
{
	return "Exit (" + std::to_string(int(this->open)) + ")";
}

void ExitTile::onInteractionStart(InteractionHandler* handler)
{
	if (this->open)
	{
		handler->completeLevel();
	}
}

void ExitTile::onButtonPressed(InteractionHandler* handler)
{
	this->open = true;

	handler->requestRedraw();
}

void ExitTile::renderDoorway(Renderer& renderer, double dx, double width)
{
	double x = this->position.x + 0.5 + dx;
	double y = this->position.y;

	renderer.setLineWidth(0.1);
	renderer.fillPolygon({
		Vector2(x, y),
		Vector2(x, y + 1),

		Vector2(x + width * 0.75, y + 1),
		Vector2(x + width, y + 0.75),
		
		Vector2(x + width, y + 0.25),
		Vector2(x + width * 0.75, y),
	});

	renderer.setLineWidth(0.15);
	renderer.drawLine(Vector2(x, y + 0.5), Vector2(x, y + 1));
	renderer.drawLine(Vector2(x, y + 1), Vector2(x + width * 0.75, y + 1));
	renderer.drawLine(Vector2(x + width * 0.75, y + 1), Vector2(x + width, y + 0.75));
}

void ExitTile::renderHighlight(Renderer& renderer, double width, Color effect, Color background)
{
	const double dur = 2;
	const int n = 5;

	double x = this->position.x + 0.5;
	double y = this->position.y + 1;

	renderer.setFillColor(effect);
	renderer.fillRect(x - width / 2, y - 0.1, width, 0.1);

	for (int i = 0; i < n; i++)
	{
		double p = normalize(fmod(this->time + i * dur / n, dur), 0, dur);

		double bottom = y - pow(p, 3) * 0.8;
		double height = pow(1.0 - p, 0.5) * 0.1;

		
		renderer.setFillColor(effect.mix(background, p*p));
		renderer.fillRect(x - width / 2, bottom - height, width, height);
	}
}


////////////////////////////////////////////////////


CoinTile::CoinTile()
{
	this->setPosition({ 0 });
}

Tile* CoinTile::clone()
{
	return new CoinTile(*this);
}

void CoinTile::setPosition(Vector2i position)
{
	this->position = position;
	Vector2 center = Vector2(0.5) + position;

	double x = center.x;
	double y = center.y;

	double width = 0.26;
	double height = 0.26;

	this->hull = ConvexHull({
		Vector2(x - width, y - height),
		Vector2(x + width, y - height),
		Vector2(x + width, y + height),
		Vector2(x - width, y + height),
	});
}

void CoinTile::render(Renderer& renderer)
{
	BoundingBox bounds = this->hull.getBoundingBox();

	bounds.left += 0.06;
	bounds.right -= 0.06;
	bounds.top += 0.06;
	bounds.bottom -= 0.06;


	renderer.setColor(255, 255, 20);
	renderer.setFillColor(150, 150, 20);
	renderer.setLineWidthAbsolute(1);
	renderer.setLineStyle(LINE_SOLID);
	
	double width = bounds.right - bounds.left;
	double height = bounds.bottom - bounds.top;

	renderer.fillRect(bounds.left, bounds.top, width, height);

	double margin = width / 4.5;

	renderer.setFillColor(255, 255, 100);
	renderer.fillRect(bounds.left + margin, bounds.top + margin, width - 2 * margin, height - 2 * margin);
}

Vector2* CoinTile::overlap(const ConvexHull& other) const
{
	return this->hull.overlap(other);
}

bool CoinTile::isPassable() const
{
	return true;
}

std::string CoinTile::getFormattedName() const
{
	return "Coin ()";
}

void CoinTile::onInteractionStart(InteractionHandler* handler)
{
	BoundingBox bounds = this->hull.getBoundingBox();

	double width = bounds.right - bounds.left;
	double height = bounds.bottom - bounds.top;

	Vector2 center = this->hull.average();
	handler->spawnEffect(new Evaporation(center, width, height));
	handler->setTile(this->position, nullptr);

	handler->increaseEnergy(1.0);
}


////////////////////////////////////////////////////


InactiveMine::InactiveMine() :
	triggered(false)
{
	this->setPosition({ 0 });
}

Tile* InactiveMine::clone()
{
	return new InactiveMine(*this);
}

void InactiveMine::setPosition(Vector2i position)
{
	this->position = position;
	Vector2 center = Vector2(0.5) + position;

	std::vector<Vector2> points;
	for (int i = 0; i < 8; i++)
	{
		Vector2 delta = MINE_RADIUS * Vector2::rotated(45 * i);
		points.push_back(delta + center);
	}
	this->hull = ConvexHull(points);
}

void InactiveMine::render(Renderer& renderer)
{
	Vector2 center = Vector2(this->hull.average());
	
	renderer.setLineWidth(0.1);
	renderer.setLineStyle(LINE_SOLID);

	if (this->triggered)
	{
		renderer.setColor(200, 0, 100);
		renderer.setFillColor(200, 0, 150);
		renderer.fillCircle(center.x, center.y, MINE_RADIUS / 1.5);
	} else
	{
		renderer.setColor(150, 0, 100);
		renderer.drawCircle(center.x, center.y, MINE_RADIUS / 1.5);
	}
}

Vector2* InactiveMine::overlap(const ConvexHull& other) const
{
	return this->hull.overlap(other);
}

bool InactiveMine::isPassable() const
{
	return true;
}

std::string InactiveMine::getFormattedName() const
{
	return "InactiveMine ()";
}

void InactiveMine::onInteractionStart(InteractionHandler* handler)
{
	this->triggered = true;
	handler->requestRedraw();
}

void InactiveMine::onInteractionEnd(InteractionHandler* handler)
{
	handler->setTile(this->position, new ActiveMine());
}


////////////////////////////////////////////////////


ActiveMine::ActiveMine()
{
	this->setPosition({ 0 });
}

Tile* ActiveMine::clone()
{
	return new ActiveMine(*this);
}

void ActiveMine::setPosition(Vector2i position)
{
	this->position = position;
	Vector2 center = Vector2(0.5) + position;

	std::vector<Vector2> points;
	for (int i = 0; i < 8; i++)
	{
		Vector2 delta = MINE_RADIUS * Vector2::rotated(45 * i);
		points.push_back(delta + center);
	}
	this->hull = ConvexHull(points);
}

void ActiveMine::render(Renderer& renderer)
{
	Vector2 center = Vector2(this->hull.average());

	renderer.setColor(200, 0, 150);
	renderer.setFillColor(200, 0, 150);
	renderer.setLineWidth(0.1);
	renderer.setLineStyle(LINE_SOLID);

	for (int i = 0; i < 8; i++)
	{
		Vector2 delta = MINE_RADIUS / 1.25 * Vector2::rotated(45 * i);

		renderer.drawLine(center, center + delta);
	}
}

Vector2* ActiveMine::overlap(const ConvexHull& other) const
{
	return this->hull.overlap(other);
}

bool ActiveMine::isPassable() const
{
	return true;
}

std::string ActiveMine::getFormattedName() const
{
	return "ActiveMine ()";
}

void ActiveMine::onInteractionStart(InteractionHandler* handler)
{
	handler->killNinja(CauseOfDeath::EXPLOSION);
	handler->spawnEffect(new Explosion(this->hull.average(), 10));
	handler->setTile(this->position, nullptr);
}


////////////////////////////////////////////////////


ButtonTile::ButtonTile()
{
	this->setPosition({ 0 });
}

Tile* ButtonTile::clone()
{
	return new ButtonTile(*this);
}

void ButtonTile::setPosition(Vector2i position)
{
	this->position = position;
	Vector2 center = Vector2(0.5) + position;

	this->hull = ConvexHull({
		center + Vector2(-w, -h),
		center + Vector2(w, -h),
		center + Vector2(w, h),
		center + Vector2(-w, h),
	});
}

void ButtonTile::render(Renderer& renderer)
{
	if (this->triggered) {
		renderer.setColor(255, 0, 0);
	} else {
		renderer.setColor(50, 200, 200);
	}

	renderer.setFillColor(200, 0, 150);
	renderer.setLineWidth(0.1);
	renderer.setLineStyle(LINE_SOLID);

	Vector2 center = Vector2(this->hull.average());
	renderer.drawRect(center.x - w, center.y - h, 2 * w, 2 * h);
}

Vector2* ButtonTile::overlap(const ConvexHull& other) const
{
	return this->hull.overlap(other);
}

bool ButtonTile::isPassable() const
{
	return true;
}

std::string ButtonTile::getFormattedName() const
{
	return "Button ()";
}

void ButtonTile::onInteractionStart(InteractionHandler* handler)
{
	handler->buttonTriggered();

	this->triggered = true;
}



////////////////////////////////////////////////////


RocketTile::RocketTile() :
	cooldown(1), canFire(true)
{
	this->setPosition({ 0 });
}

Tile* RocketTile::clone()
{
	return new RocketTile(*this);
}

void RocketTile::setPosition(Vector2i position)
{
	this->position = position;
}

void RocketTile::update(InteractionHandler* handler, double deltaTime)
{
	if (canFire)
	{
		Vector2 center = Vector2(this->position) + Vector2(0.5);
		
		ConvexHull line = ConvexHull::newLine(center, handler->getNinjaPosition(), 0.01);

		if (Vector2* overlap = handler->getBlockOverlap(line)) {
			delete overlap;
			this->cooldown = 1;
		} else {
			this->cooldown = clamp(this->cooldown - deltaTime * 1.0, 0.0, 1.0);

			if (this->cooldown == 0)
			{
				double speed = 6;
				switch (handler->getDifficulty()) {
				case EASY: speed = 6; break;
				case NORMAL: speed = 7; break;
				case HARD: speed = 8; break;
				}

				Vector2 center = Vector2(this->position) + Vector2(0.5);
				Vector2 velocity = speed * (handler->getNinjaPosition() - center).normal();

				handler->spawnEntity(new Rocket(this, center, velocity));

				this->cooldown = 1;
				this->canFire = false;
			}
		}
	}

}

void RocketTile::render(Renderer& renderer)
{
	Vector2 center = Vector2(this->position) + Vector2(0.5);

	renderer.setColor(100, 100, 200);
	renderer.setFillColor(100, 100, 200);

	renderer.setLineStyle(LINE_SOLID);
	renderer.setLineWidth(1.0 / 20);

	renderer.drawCircle(center.x, center.y, 0.3);

	renderer.setLineStyle(LINE_NONE);
	renderer.fillCircle(center.x, center.y, lerp(this->cooldown, 0.3, 0.0));
}

Vector2* RocketTile::overlap(const ConvexHull& other) const
{
	return nullptr;
}

bool RocketTile::isPassable() const
{
	return true;
}

bool RocketTile::isDynamic() const
{
	return true;
}

std::string RocketTile::getFormattedName() const
{
	return "Rocket ()";
}

void RocketTile::onRocketExplode()
{
	this->canFire = true;
}
