#pragma once
#include "InteractionHandler.h"
#include "Collider.h"

class Entity: public Collider
{
public:
	
	virtual ~Entity() = default;

	// Uppdaterar entiteten
	virtual void update(InteractionHandler* handler, double deltaTime) = 0;


	// Ritar entiteten
	virtual void render(Renderer& renderer) = 0;


	// Returnerar den minsta förflyttning som krävs för att undvika en kollision
	virtual Vector2* overlap(const ConvexHull& other) const override = 0;

	// Returnerar den minsta förflyttning som krävs för att undvika en kollision
	virtual Vector2* overlap(const Collider* other) const = 0;


	// Returnerar en fyrkant som omsluter denna entitet
	virtual BoundingBox getBoundingBox() const = 0;


	// Anropas när entiteten kolliderade med ett objekt
	virtual void onCollide(InteractionHandler* handler) {}

	// Anropas när entiteten kolliderade med ninjan
	virtual void onNinjaCollide(InteractionHandler* handler) {}

	
	// Avgör om entiteten finns kvar
	virtual bool isAlive() = 0;
};
