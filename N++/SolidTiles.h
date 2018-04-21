#pragma once

#include "Tile.h"

class SquareTile : public Tile {

	ConvexHull hull;

public:

	SquareTile();

	Tile* clone() override;

	void setPosition(Vector2i position) override;

	void render(Renderer& renderer) override;

	Vector2* overlap(const ConvexHull& other) const override;

	bool isPassable() const override;

	std::string getFormattedName() const override;
};


class WedgeTile : public Tile {

	int type;
	ConvexHull hull;

public:

	WedgeTile(int type);

	Tile* clone() override;

	void setPosition(Vector2i position) override;

	void render(Renderer& renderer) override;

	Vector2* overlap(const ConvexHull& other) const override;

	bool isPassable() const override;
	
	std::string getFormattedName() const override;
};
