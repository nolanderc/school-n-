#include "SolidTiles.h"

SquareTile::SquareTile()
{
	setPosition({ 0, 0 });
}

Tile * SquareTile::clone()
{
	Tile* copy = new SquareTile(*this);
	return copy;
}


void SquareTile::setPosition(Vector2i position)
{
	double x = position.x;
	double y = position.y;

	hull = ConvexHull({
		{ x, y },
		{ x + 1, y },
		{ x + 1, y + 1 },
		{ x, y + 1 },
		});
}

void SquareTile::render(Renderer& renderer)
{
	this->hull.fill(renderer);
}

Vector2* SquareTile::overlap(const ConvexHull& other) const
{
	return this->hull.overlap(other);
}

bool SquareTile::passable() const
{
	return false;
}



WedgeTile::WedgeTile(int type)
	: type(type)
{
	setPosition({ 0, 0 });
}

Tile * WedgeTile::clone()
{
	Tile* copy = new WedgeTile(*this);
	return copy;
}

void WedgeTile::setPosition(Vector2i position)
{
	double x = position.x;
	double y = position.y;

	std::vector<Vector2> corners = {
		{ x, y },
	{ x + 1, y },
	{ x + 1, y + 1 },
	{ x, y + 1 },
	};

	corners.erase(corners.begin() + type);

	hull = ConvexHull(corners);
}

void WedgeTile::render(Renderer& renderer)
{
	this->hull.fill(renderer);
}

Vector2* WedgeTile::overlap(const ConvexHull& other) const
{
	return this->hull.overlap(other);
}

bool WedgeTile::passable() const
{
	return false;
}
