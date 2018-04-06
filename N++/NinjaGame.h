#pragma once
#include "App.h"

#include "ConvexHull.h"
#include "Level.h"

#include "Ninja.h"

#define TILE_SIZE 32

class NinjaGame: public App
{
	Level level;

	Vector2i* selectedTile;
	Tile* currentTile;

	Bitmap levelBitmap;

	// Borde nivån ritas om?
	bool renderLevel;



public:
	NinjaGame();


	void update(float dt) override;

	void draw(Renderer & renderer) override;

protected:

	void sizeChanged(int width, int height) override;

	void keyPressed(int key) override;
	void keyReleased(int key) override;

	void mousePressed(MouseButton button, int x, int y) override;

	void mouseMoved(int x, int y) override;


private:

	void updateFrameCounter(double deltaTime);


};

