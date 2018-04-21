#pragma once
#include "Collider.h"
#include "Effect.h"
#include "Entities.h"

#include "InteractionHandler.h"

// Ett block i en niv�
class Tile : public Collider
{
public:

	virtual ~Tile() = default;

	// Skapar en kopia av detta block
	virtual Tile* clone() = 0;

	// S�tter blockets position
	virtual void setPosition(Vector2i position) = 0;

	// Uppdaterar blocket
	virtual void update(InteractionHandler* handler, double deltaTime) {}

	// Ritar detta block
	virtual void render(Renderer& renderer) = 0;

	// Returnerar den minsta f�rflyttning som kr�vs f�r att undvika en kollision
	Vector2* overlap(const ConvexHull& other) const override = 0;


	// Avg�r om objekt kan r�ra sig genom detta block
	virtual bool isPassable() const { return false; }


	// Avg�r om detta block �r dynamiskt (�ndras ofta)
	virtual bool isDynamic() const { return false; };


	// Anropas n�r spelaren kommer i kontakt med detta block
	virtual void onInteractionStart(InteractionHandler* handler) {}


	// Anropas n�r spelaren avslutar sin kontakt med detta block
	virtual void onInteractionEnd(InteractionHandler* handler) {}


	// Anropas n�r en knapp trycktes
	virtual void onButtonPressed(InteractionHandler* handler) {}


	// Returnera en string (i ett lvl-format med parametrar)
	virtual std::string getFormattedName() const = 0;
};


