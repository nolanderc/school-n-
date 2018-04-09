#pragma once

#include "App.h"

#include "Level.h"

#define TILE_SIZE 32

#define PALETTE_WIDTH_PIXELS 128
#define PALETTE_WIDTH 2

const SIZE LEVEL_SIZE = { 45, 25 };
const SIZE LEVEL_SIZE_PIXELS = { LEVEL_SIZE.cx * TILE_SIZE, LEVEL_SIZE.cy * TILE_SIZE };

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
	
	// Vilket block muspekaren markerar
	Vector2i* currentTileCoord;


	// Borde ett rutnät ritas ut?
	bool grid;


	// En palett av alla block som kan placeras
	std::vector<Tile*> tilePalette;

public: 

	LevelEditor(App* parent);



private:

	// Skapar alla block som går att placera och sparar dem i paletten
	void createPalette();


	// Ritar nivån
	void drawLevel(Renderer& renderer);

	// Ritar ett rutnät
	void drawGrid(Renderer& renderer);

	// Ritar markeringen och markören
	void drawSelection(Renderer& renderer);

	
	// Ritar ut paletten
	void drawPalette(Renderer& renderer);


protected:

	void update(float deltaTime) override;

	void draw(Renderer& renderer) override;

	void keyPressed(int key) override;

	void mouseMoved(int x, int y) override;

	void mousePressed(MouseButton button, int x, int y) override;
	void mouseReleased(MouseButton button, int x, int y) override;

};


// Returnerar en rektangel som omsluter en markering
RECT getSelectionBounds(Vector2i start, Vector2i end);


// Returnerar alla koordinater innanför två motsatta hörn på en rektangel/innanför en markering
std::vector<Vector2i> getSelectionCoords(Vector2i start, Vector2i end);
