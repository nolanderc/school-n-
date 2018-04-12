﻿#pragma once
#include "Tile.h"

#include "SolidTiles.h"
#include "FunctionalTiles.h"
#include "Ninja.h"

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
Tile * createTileFromID(TileID id);


class TilePalette
{
	// Den typ av block som är vald
	const TileID* currentTile;

	// En palett av alla block som kan placeras
	std::vector<TileID> tiles;

	// Antalet kolumner
	int columns;

	
	// Avståndet mellan blocken i paletten
	double margin;

public:
	
	TilePalette(int columns, double width);


	// Ritar paletten
	void draw(Renderer& renderer);


	// Väljer ett block i paletten
	void selectTile(Vector2i coord);

	// Returnerar det nuvarande blocket
	TileID getCurrentTile();

private:
	
	// Ritar ett block utifrån dess id
	void renderTileFromID(Renderer& renderer, TileID id);
};


// Ritar en ninja vid en koordinat
void renderNinja(Renderer& renderer, Vector2 position);
