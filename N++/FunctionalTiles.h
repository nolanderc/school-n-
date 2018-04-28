#pragma once
#include "Tile.h"
#include "Explosion.h"
#include "Evaporation.h"
#include "Entities.h"

#define MINE_RADIUS 0.3

class ExitTile : public Tile
{
	Vector2i position;

	bool open;

	double openAmount;

	double time;

public:

	ExitTile(bool open);

	Tile* clone() override;

	void setPosition(Vector2i position) override;

	void update(InteractionHandler* handler, double deltaTime) override;

	void render(Renderer& renderer) override;

	Vector2* overlap(const ConvexHull& other) const override;

	bool isPassable() const override;

	bool isDynamic() const override;


	std::string getFormattedName() const override;


	void onInteractionStart(InteractionHandler* handler) override;

	void onButtonPressed(InteractionHandler* handler) override;

private:

	// Rita en dörr med ett visst avstånd ifrån mitten
	void renderDoorway(Renderer& renderer, double dx, double width);

	// Ritar en effekt i dörrkarmen
	void renderHighlight(Renderer& renderer, double width, Color effect, Color background);
};



class CoinTile : public Tile
{
	Vector2i position;

	ConvexHull hull;

public:

	CoinTile();

	Tile* clone() override;

	void setPosition(Vector2i position) override;

	void render(Renderer& renderer) override;

	Vector2* overlap(const ConvexHull& other) const override;

	bool isPassable() const override;

	std::string getFormattedName() const override;


	void onInteractionStart(InteractionHandler* handler) override;
};



class InactiveMine : public Tile
{
	Vector2i position;

	ConvexHull hull;

	bool triggered;

public:

	InactiveMine();

	Tile* clone() override;

	void setPosition(Vector2i position) override;

	void render(Renderer& renderer) override;

	Vector2* overlap(const ConvexHull& other) const override;

	bool isPassable() const override;

	std::string getFormattedName() const override;


	void onInteractionStart(InteractionHandler* handler) override;

	void onInteractionEnd(InteractionHandler* handler) override;
};


class ActiveMine: public Tile
{
	Vector2i position;

	ConvexHull hull;

public:

	ActiveMine();

	Tile* clone() override;

	void setPosition(Vector2i position) override;

	void render(Renderer& renderer) override;

	Vector2* overlap(const ConvexHull& other) const override;

	bool isPassable() const override;

	std::string getFormattedName() const override;


	void onInteractionStart(InteractionHandler* handler) override;
};


class ButtonTile : public Tile
{
	Vector2i position;

	ConvexHull hull;

	bool triggered;

	const double w = 0.2;
	const double h = 0.1;

public:

	ButtonTile();

	Tile* clone() override;

	void setPosition(Vector2i position) override;

	void render(Renderer& renderer) override;

	Vector2* overlap(const ConvexHull& other) const override;

	bool isPassable() const override;

	std::string getFormattedName() const override;


	void onInteractionStart(InteractionHandler* handler) override;
};



class RocketTile : public Tile
{
	Vector2i position;

	double cooldown;

	bool canFire;

public:

	RocketTile();

	Tile* clone() override;

	void setPosition(Vector2i position) override;


	void update(InteractionHandler* handler, double deltaTime) override;

	void render(Renderer& renderer) override;

	Vector2* overlap(const ConvexHull& other) const override;

	bool isPassable() const override;

	bool isDynamic() const override;

	std::string getFormattedName() const override;

	// Anropas när raketen som har avfyrats har exploderat
	void onRocketExplode();
};