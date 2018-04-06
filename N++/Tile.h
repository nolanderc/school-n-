#pragma once
#include "Collider.h"
#include "Effect.h"

class InteractionHandler;


// Ett block i en niv�
class Tile : public Collider
{
public:

	virtual ~Tile() = default;

	// Skapar en kopia av detta block
	virtual Tile* clone() = 0;

	// S�tter blockets position
	virtual void setPosition(Vector2i position) = 0;

	// virtual const ConvexHull& getConvexHull() const = 0;
	// Ritar detta block
	virtual void render(Renderer& renderer) = 0;

	// Returnerar den minsta f�rflyttning som kr�vs f�r att undvika en kollision
	Vector2* overlap(const ConvexHull& other) const override = 0;

	// Avg�r om objekt kan r�ra sig genom detta block
	virtual bool passable() const = 0;


	// Anropas n�r spelaren kommer i kontakt med detta block
	virtual void onInteractionStart(InteractionHandler* handler) {}


	// Anropas n�r spelaren avslutar sin kontakt med detta block
	virtual void onInteractionEnd(InteractionHandler* handler) {}
};


enum CauseOfDeath
{
	EXPLOSION
};

// En klass som hanterar interaktioner med block
class InteractionHandler
{
public:

	// Avslutar niv�n med en vinst
	virtual void completeLevel() = 0;

	// D�dar ninjan (p� valfritt s�tt)
	virtual void killNinja(CauseOfDeath causeOfDeath) = 0;

	// Ers�tter ett block vid en viss koordinat
	virtual void setTile(Vector2i coord, Tile* tile) = 0;


	// Skapar en ny effekt
	virtual void spawnEffect(Effect* effect) = 0;


	// Anropas n�r alla block borde ritas om
	virtual void requestRedraw() = 0;
};
