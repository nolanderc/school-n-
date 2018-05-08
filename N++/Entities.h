#pragma once
#include "Entity.h"
#include "Explosion.h"

class RocketTile;

class Rocket: public Entity
{
	ConvexHull hull;

	Vector2 position, velocity;

	bool alive;

	RocketTile* parent;

public:

	Rocket(RocketTile* parent, Vector2 position, Vector2 velocity);
	~Rocket();

	void update(InteractionHandler* handler, double deltaTime) override;

	void render(Renderer& renderer) override;

	Vector2* overlap(const ConvexHull& other) const override;
	Vector2* overlap(const Collider* other) const override;

	BoundingBox getBoundingBox() const override;

	bool isAlive() override;


	void onCollide(InteractionHandler* handler) override;

	void onNinjaCollide(InteractionHandler* handler) override;
};
