#pragma once

#include "Level.h"


struct Keyframe
{
	// När detta kommando ska utföras
	double time;

	// Hur länge detta kommando ska utföras
	double duration;

	// Kommandot som ska utföras
	NinjaMovement command;

	Keyframe(double time, double duration, NinjaMovement command);
};


class Animation
{
	Level level;

	std::vector<Keyframe> frames;

	double duration;

	double time;
	double accumulator;

	bool loop;

public:
	
	Animation(Level level, std::vector<Keyframe> keyframes, double duration, bool loop);


	void update(double deltaTime);

	void render(Renderer& renderer, Color background);

	int getWidth();
	int getHeight();
};
