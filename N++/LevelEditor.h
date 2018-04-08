#pragma once

#include "App.h"

#include "Level.h"

#define TILE_SIZE 32

const SIZE LEVEL_SIZE = { 45, 25 };

class LevelEditor : public App
{
	// Nivån som håller på att skapas
	Level level;

	// En bitmap att rita nivån till
	Bitmap levelBitmap;


	// De block som är markerade
	Vector2i* selectionStart;
	Vector2i selectionEnd;

	// Den typ av block som är vald
	Tile* currentTile;


	// Borde ett rutnät ritas ut?
	bool grid;

public: 

	LevelEditor(App* parent);



private:

	// Ritar nivån
	void drawLevel(Renderer& renderer);

	// Ritar ett rutnät
	void drawGrid(Renderer& renderer);

	// Ritar markeringen och markören
	void drawSelection(Renderer& renderer);

protected:

	void update(float deltaTime) override;

	void draw(Renderer& renderer) override;

	void keyPressed(int key) override;

	void mouseMoved(int x, int y) override;

	void mousePressed(MouseButton button, int x, int y) override;
	void mouseReleased(MouseButton button, int x, int y) override;

};


// Returnerar alla koordinater innanför två motsatta hörn på en rektangel/innanför en markering
std::vector<Vector2i> getSelectionCoords(Vector2i start, Vector2i end);
