#pragma once
#include "App.h"

#include "ConvexHull.h"
#include "Level.h"

#include "Ninja.h"

#define TILE_SIZE 32

class NinjaGame: public App
{
	// Nivån
	Level level;

	// Bitmap att rita nivån till
	Bitmap levelBitmap;

	// Borde nivån ritas om?
	bool renderLevel;



public:

	NinjaGame(App* parent, std::string levelPath);


	void update(float dt) override;

	void draw(Renderer & renderer) override;

protected:

	void sizeChanged(int width, int height) override;

	void keyPressed(int key) override;
	void keyReleased(int key) override;

private:

	void updateFrameCounter(double deltaTime);


};

