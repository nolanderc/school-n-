#pragma once

#include "App.h"

#include "Level.h"

#define TILE_SIZE 32

#define PALETTE_WIDTH_PIXELS 128
#define PALETTE_WIDTH 2

const SIZE LEVEL_SIZE = { 45, 25 };
const SIZE LEVEL_SIZE_PIXELS = { LEVEL_SIZE.cx * TILE_SIZE, LEVEL_SIZE.cy * TILE_SIZE };

const double PALETTE_MARGIN = (double(PALETTE_WIDTH_PIXELS) / TILE_SIZE - PALETTE_WIDTH) / double(PALETTE_WIDTH + 1);



// Nummer som identifierar ett block
enum TileID
{
	TILE_SQUARE,
	TILE_PLAYER_START,
	TILE_WEDGE0,
	TILE_WEDGE1,
	TILE_WEDGE2,
	TILE_WEDGE3,
	TILE_MINE_ACTIVE,
	TILE_MINE_INACTIVE,
	TILE_EXIT,
	TILE_COIN
};


// Skapar ett nytt block ifrån ett id
Tile* createTileFromID(TileID id);


// Ritar ett block utifrån dess id
void renderTileFromID(Renderer& renderer, TileID id);


// Ritar en ninja vid en koordinat
void renderNinja(Renderer& renderer, Vector2 position);


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
	const TileID* currentTile;
	
	// Vilket block muspekaren markerar
	Vector2i* currentTileCoord;


	// Borde ett rutnät ritas ut?
	bool grid;


	// En palett av alla block som kan placeras
	std::vector<TileID> tilePalette;

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

	// Returnerar det block i paletten som finns under musens koordinater (relativt till skärmen)
	const TileID* selectPaletteTile(Vector2i mouse);

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
