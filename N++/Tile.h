#pragma once
#include "Collider.h"
#include "Effect.h"
#include "Entities.h"

#include "InteractionHandler.h"

// Ett block i en nivå
class Tile : public Collider
{
public:

	virtual ~Tile() = default;

	// Skapar en kopia av detta block
	virtual Tile* clone() = 0;

	// Sätter blockets position
	virtual void setPosition(Vector2i position) = 0;

	// Uppdaterar blocket
	virtual void update(InteractionHandler* handler, double deltaTime) {}

	// Ritar detta block
	virtual void render(Renderer& renderer) = 0;

	// Returnerar den minsta förflyttning som krävs för att undvika en kollision
	Vector2* overlap(const ConvexHull& other) const override = 0;


	// Avgör om objekt kan röra sig genom detta block
	virtual bool isPassable() const { return false; }


	// Avgör om detta block är dynamiskt (ändras ofta)
	virtual bool isDynamic() const { return false; };


	// Anropas när spelaren kommer i kontakt med detta block
	virtual void onInteractionStart(InteractionHandler* handler) {}


	// Anropas när spelaren avslutar sin kontakt med detta block
	virtual void onInteractionEnd(InteractionHandler* handler) {}


	// Anropas när en knapp trycktes
	virtual void onButtonPressed(InteractionHandler* handler) {}


	// Returnera en string (i ett lvl-format med parametrar)
	virtual std::string getFormattedName() const = 0;
};


