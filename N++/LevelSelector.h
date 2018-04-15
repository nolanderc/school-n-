#pragma once
#include "App.h"
#include "Level.h"

#define TILE_SIZE 8
#define TILE_MARGIN 5

class LevelSelector: public App
{
	std::vector<Level> levels;

	int* selectedLevel;
	double zoomAmount = 0;

public:
	

	LevelSelector(App* parent);

protected:

	void update(float deltaTime) override;

	void draw(Renderer& renderer) override;


	void keyPressed(int key) override;
};
