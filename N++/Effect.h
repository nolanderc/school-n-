﻿#pragma once

#include "Renderer.h"

// En gemensam API för alla effekter
class Effect
{
public:
	
	// Avgör om effekten fortfarande finns
	virtual bool isAlive() = 0;

	// Uppdaterar effekten
	virtual void update(double deltaTime) = 0;

	// Ritar effekten
	virtual void render(Renderer& renderer) = 0;

};