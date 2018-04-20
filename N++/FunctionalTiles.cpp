#include "FunctionalTiles.h"

////////////////////////////////////////////////////

ExitTile::ExitTile(bool open) :
	open(open)
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

void ExitTile::render(Renderer& renderer)
{
	renderer.setFillColor(150, 150, 150);
	this->renderDoorway(renderer, 0);

	if (this->open)
	{
		renderer.setFillColor(200, 0, 0);
		this->renderDoorway(renderer, 0.05);
	
		renderer.setFillColor(200, 200, 0);
		this->renderDoorway(renderer, 0.15);
	}
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

bool CoinTile::passable() const
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

bool InactiveMine::passable() const
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

bool ActiveMine::passable() const
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
	handler->spawnEffect(new Explosion(this->hull.average()));
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
	Vector2 center = Vector2(this->hull.average());

	if (this->triggered)
	{
		renderer.setColor(255, 0, 0);
	} else
	{
		renderer.setColor(100, 150, 100);
	}
	renderer.setFillColor(200, 0, 150);
	renderer.setLineWidth(0.1);
	renderer.setLineStyle(LINE_SOLID);

	renderer.drawRect(center.x - w, center.y - h, 2 * w, 2 * h);
}

Vector2* ButtonTile::overlap(const ConvexHull& other) const
{
	return this->hull.overlap(other);
}

bool ButtonTile::passable() const
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

