#include "FunctionalTiles.h"

ExitTile::ExitTile()
{
}

Tile* ExitTile::clone()
{
	return new ExitTile(*this);
}

void ExitTile::setPosition(Vector2i position)
{
	this->position = position;
}

void ExitTile::render(Renderer& renderer)
{
	renderer.setFillColor(150, 150, 150);
	this->renderDoorway(renderer, 0);

	renderer.setFillColor(200, 0, 0);
	this->renderDoorway(renderer, 0.05);
	
	renderer.setFillColor(200, 200, 0);
	this->renderDoorway(renderer, 0.15);
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

bool ExitTile::passable() const
{
	return true;
}

void ExitTile::onInteractionStart(InteractionHandler* handler)
{
	handler->completeLevel();
}

void ExitTile::renderDoorway(Renderer& renderer, double margin)
{
	double x = this->position.x;
	double y = this->position.y;

	renderer.fillPolygon({
		Vector2(x + margin, y + 0.25 + margin),
		Vector2(x + 0.25 + margin, y + margin),
		Vector2(x + 0.75 - margin, y + margin),
		Vector2(x + 1 - margin, y + 0.25 + margin),

		Vector2(x + 1 - margin, y + 1),
		Vector2(x + margin, y + 1),
	});
}

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

	double width = 0.2;
	double height = 0.2;

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

bool CoinTile::passable() const
{
	return true;
}

void CoinTile::onInteractionStart(InteractionHandler* handler)
{
	BoundingBox bounds = this->hull.getBoundingBox();

	double width = bounds.right - bounds.left;
	double height = bounds.bottom - bounds.top;

	Vector2 center = this->hull.average();
	handler->spawnEffect(new Evaporation(center, width, height));
	handler->setTile(this->position, nullptr);
}

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
	
	renderer.setColor(200, 0, 150);
	renderer.setFillColor(200, 0, 150);
	renderer.setLineWidthAbsolute(2);
	renderer.setLineStyle(LINE_SOLID);

	if (this->triggered)
	{
		renderer.fillCircle(center.x, center.y, MINE_RADIUS / 1.5);
	} else
	{
		renderer.drawCircle(center.x, center.y, MINE_RADIUS / 1.5);
	}
}

Vector2* InactiveMine::overlap(const ConvexHull& other) const
{
	return this->hull.overlap(other);
}

bool InactiveMine::passable() const
{
	return true;
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
	renderer.setLineWidthAbsolute(3);
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

bool ActiveMine::passable() const
{
	return true;
}

void ActiveMine::onInteractionStart(InteractionHandler* handler)
{
	handler->killNinja(CauseOfDeath::EXPLOSION);
	handler->spawnEffect(new Explosion(this->hull.average()));
	handler->setTile(this->position, nullptr);
}
