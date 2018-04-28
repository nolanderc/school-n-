#include "TilePalette.h"


TilePalette::TilePalette(int columns, double width)
{
	this->tiles = {
		TILE_SQUARE,
		TILE_PLAYER_START,
		TILE_WEDGE0,
		TILE_WEDGE1,
		TILE_WEDGE3,
		TILE_WEDGE2,
		TILE_MINE_ACTIVE,
		TILE_MINE_INACTIVE,
		TILE_EXIT_CLOSED,
		TILE_EXIT_OPEN,
		TILE_BUTTON,
		TILE_COIN,
		TILE_ROCKET
	};

	this->currentTile = &this->tiles[0];

	this->columns = columns;

	this->margin = (width - columns) / double(columns);
}

void TilePalette::draw(Renderer& renderer)
{
	int tileCount = this->tiles.size();

	for (int i = 0; i < tileCount; i++)
	{
		int x = i % this->columns;
		int y = i / this->columns;

		Vector2 delta = { this->margin + x * (this->margin + 1) - 0.5, this->margin + y * (this->margin + 1) - 0.5 };
		renderer.offset(delta);
		renderer.setLineStyle(LINE_NONE);


		TileID* id = &this->tiles[i];

		if (id == this->currentTile)
		{
			renderer.setFillColor(42, 200, 200);
			renderer.fillRect(-this->margin / 2, -this->margin / 2, 1 + this->margin, 1 + this->margin);
		}

		renderer.setColor(0, 0, 0);
		renderer.setFillColor(0, 0, 0);

		renderTileFromID(renderer, *id);

		renderer.offset(-delta);
	}
}

void TilePalette::selectTile(Vector2i coord)
{
	if (0 <= coord.x && coord.x < this->columns && 0 <= coord.y)
	{
		int	index = coord.x + coord.y * this->columns;

		if (index < this->tiles.size())
		{
			this->currentTile = &this->tiles[index];
		}
	}
}

TileID TilePalette::getCurrentTile()
{
	return *this->currentTile;
}

void TilePalette::setCurrentTile(TileID tileId)
{
	int count = this->tiles.size();
	for (int i = 0; i < count; i++)
	{
		if (this->tiles[i] == tileId)
		{
			this->currentTile = &this->tiles[i];
			return;
		}
	}
}

Tile* createTileFromID(TileID id)
{
	switch (id)
	{
	case TILE_PLAYER_START: return nullptr;

	case TILE_SQUARE: return new SquareTile();

	case TILE_WEDGE0: return new WedgeTile(0);
	case TILE_WEDGE1: return new WedgeTile(1);
	case TILE_WEDGE2: return new WedgeTile(2);
	case TILE_WEDGE3: return new WedgeTile(3);

	case TILE_MINE_ACTIVE: return new ActiveMine();
	case TILE_MINE_INACTIVE: return new InactiveMine();

	case TILE_COIN: return new CoinTile();

	case TILE_EXIT_OPEN: return new ExitTile(true);
	case TILE_EXIT_CLOSED: return new ExitTile(false);

	case TILE_BUTTON: return new ButtonTile();

	case TILE_ROCKET: return new RocketTile();


	default: return nullptr;
	}
}

TileID createTileIdFromName(const std::string& name)
{
	std::string n = toLower(name);

	if (n == "square ()") return TILE_SQUARE;

	if (n == "wedge (0)") return TILE_WEDGE0;
	if (n == "wedge (1)") return TILE_WEDGE1;
	if (n == "wedge (2)") return TILE_WEDGE2;
	if (n == "wedge (3)") return TILE_WEDGE3;

	if (n == "activemine ()") return TILE_MINE_ACTIVE;
	if (n == "inactivemine ()") return TILE_MINE_INACTIVE;

	if (n == "exit (0)") return TILE_EXIT_CLOSED;
	if (n == "exit (1)") return TILE_EXIT_OPEN;

	if (n == "coin ()") return TILE_COIN;
	
	if (n == "button ()") return TILE_BUTTON;

	if (n == "rocket ()") return TILE_ROCKET;

	return TILE_PLAYER_START;
}

void TilePalette::renderTileFromID(Renderer& renderer, TileID id)
{
	switch (id)
	{
	case TILE_PLAYER_START:
		renderNinja(renderer, { 0 });
		break;

	default: {
		Tile*tile = createTileFromID(id);
		tile->setPosition({ 0 });
		tile->render(renderer);
		delete tile;
	} break;

	}
}


void renderNinja(Renderer& renderer, Vector2 position)
{
	renderer.setLineStyle(LINE_SOLID);
	renderer.setLineWidthAbsolute(2);

	Ninja ninja(position);
	ninja.render(renderer);
}
