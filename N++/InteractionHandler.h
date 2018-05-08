#pragma once
#include "Linear.h"
#include "Difficulty.h"
#include "ConvexHull.h"

class Tile;
class Effect;
class Entity;

enum CauseOfDeath
{
	EXPLOSION
};

// En klass som hanterar interaktioner mellan objekt
class InteractionHandler
{
public:
	virtual ~InteractionHandler() = default;

	// Ger ninjan energi
	virtual void increaseEnergy(double amount) = 0;

	// Avslutar niv�n med en vinst
	virtual void completeLevel() = 0;

	// D�dar ninjan (p� valfritt s�tt)
	virtual void killNinja(CauseOfDeath causeOfDeath) = 0;

	// Ers�tter ett block vid en viss koordinat
	virtual void setTile(Vector2i coord, Tile* tile) = 0;


	// Skapar en ny effekt
	virtual void spawnEffect(Effect* effect) = 0;

	// Skapar en ny entitet (r�rande objekt)
	virtual void spawnEntity(Entity* entity) = 0;

	// Anropas n�r alla block borde ritas om
	virtual void requestRedraw() = 0;


	// Anropas n�r en knapp aktiverades
	virtual void buttonTriggered() = 0;


	// Returnerar ninjans position
	virtual Vector2 getNinjaPosition() = 0;


	// Returnerar den nuvarande sv�righetsgraden
	virtual Difficulty getDifficulty() = 0;


	// Returnerar den minsta f�rflyttningen som kr�vs f�r att flytta ett konvext skal ut ur alla block
	virtual Vector2* getBlockOverlap(const ConvexHull& hull) = 0;
};
