#pragma once
#include "Tile.h"
#include "Explosion.h"
#include "Evaporation.h"

#define MINE_RADIUS 0.3

class ExitTile : public Tile
{
	Vector2i position;

	bool open;

public:

	ExitTile(bool open);

	Tile* clone() override;

	void setPosition(Vector2i position) override;

	void render(Renderer& renderer) override;

	Vector2* overlap(const ConvexHull& other) const override;

	bool passable() const override;

	std::string getFormattedName() const override;


	void onInteractionStart(InteractionHandler* handler) override;

	void onButtonPressed(InteractionHandler* handler) override;

private:

	// Rita en dörr med ett visst avstånd ifrån "dörrkarmen"
	void renderDoorway(Renderer& renderer, double margin);

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

	bool passable() const override;

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

	bool passable() const override;

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

	bool passable() const override;

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

	bool passable() const override;

	std::string getFormattedName() const override;


	void onInteractionStart(InteractionHandler* handler) override;
};
