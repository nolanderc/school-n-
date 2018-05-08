#pragma once

#include "Renderer.h"
#include "InteractionHandler.h"

// En gemensam API för alla effekter
class Effect
{
public:
	virtual ~Effect() = default;

	// Avgör om effekten fortfarande finns
	virtual bool isAlive() = 0;

	// Uppdaterar effekten
	virtual void update(double deltaTime, InteractionHandler* handler) = 0;

	// Ritar effekten
	virtual void render(Renderer& renderer, Color background) = 0;

};
