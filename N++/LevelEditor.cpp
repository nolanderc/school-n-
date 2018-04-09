#include "LevelEditor.h"


Tile* createTileFromID(TileID id)
{
	switch (id)
	{
	case TILE_SQUARE: return new SquareTile();
	case TILE_PLAYER_START: return nullptr;
	case TILE_WEDGE0: return new WedgeTile(0);
	case TILE_WEDGE1: return new WedgeTile(1);
	case TILE_WEDGE2: return new WedgeTile(2);
	case TILE_WEDGE3: return new WedgeTile(3);
	case TILE_MINE_ACTIVE: return new ActiveMine();
	case TILE_MINE_INACTIVE: return new InactiveMine();
	case TILE_EXIT: return new ExitTile();
	case TILE_COIN: return new CoinTile();
	default:;
	}
}

void renderTileFromID(Renderer& renderer, TileID id)
{
	switch (id)
	{
	case TILE_PLAYER_START: renderNinja(renderer, { 0 }); break;

	default: {
		Tile*tile = createTileFromID(id);
		tile->setPosition({ 0 });
		tile->render(renderer);
	} break;

	}
}

void renderNinja(Renderer& renderer, Vector2 position)
{
	Ninja ninja(position);
	ninja.render(renderer);
}

LevelEditor::LevelEditor(App* parent) :
	App(parent), level(LEVEL_SIZE.cx, LEVEL_SIZE.cy), grid(true)
{
	this->setWindowSize(LEVEL_SIZE_PIXELS.cx + PALETTE_WIDTH_PIXELS, LEVEL_SIZE_PIXELS.cy);

	this->levelBitmap = this->createCompatibleBitmap(LEVEL_SIZE_PIXELS);

	this->createPalette();

	this->currentTile = &this->tilePalette[0];
}

void LevelEditor::update(float deltaTime)
{

}

void LevelEditor::draw(Renderer& renderer)
{
	renderer.scale(TILE_SIZE);

	renderer.setFillColor(100, 100, 100);
	renderer.clear();

	this->drawLevel(renderer);

	if (grid) this->drawGrid(renderer);

	this->drawSelection(renderer);

	renderer.offset(Vector2i{ this->level.getWidth(), 0 });
	this->drawPalette(renderer);
}


void LevelEditor::createPalette()
{
	this->tilePalette = {
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
}


void LevelEditor::drawLevel(Renderer& renderer)
{
	if (this->level.needsRerender())
	{
		Renderer levelRenderer = renderer.createBitmapRenderer(this->levelBitmap);
		levelRenderer.scale(TILE_SIZE);

		levelRenderer.setFillColor(255, 255, 0);
		levelRenderer.clear();

		levelRenderer.setFillColor(0, 0, 0);
		level.renderStatic(levelRenderer);

		levelRenderer.setLineStyle(LINE_SOLID);
		levelRenderer.setColor(100, 100, 100);
		Vector2 ninjaPos = this->level.getNinjaSpawn();
		renderNinja(levelRenderer, ninjaPos);

		DeleteDC(levelRenderer.releaseDC());
	}

	// Rita nivån
	renderer.drawBitmap(this->levelBitmap, 0, 0, this->levelBitmap.getWidth(), this->levelBitmap.getHeight(), 0, 0);

	// Rita en ram runt nivån
	renderer.setColor(0, 0, 0);
	renderer.setLineStyle(LINE_SOLID);
	renderer.setLineWidthAbsolute(4);
	renderer.drawRect(0, 0, this->level.getWidth(), this->level.getHeight());
}

void LevelEditor::drawGrid(Renderer& renderer)
{
	renderer.setColor(100, 100, 100);
	renderer.setLineStyle(LINE_SOLID);
	renderer.setLineWidthAbsolute(1);

	for (int x = 0; x < this->level.getWidth(); x++)
	{
		renderer.drawLine(x, 0, x, this->level.getHeight());
	}

	for (int y = 0; y < this->level.getHeight(); y++)
	{
		renderer.drawLine(0, y, this->level.getWidth(), y);
	}
}

void LevelEditor::drawSelection(Renderer& renderer)
{
	if (this->currentTile)
	{
		renderer.setFillColor(200, 200, 20);
		renderer.setColor(0, 0, 255);
		renderer.setLineStyle(LINE_SOLID);
		renderer.setLineWidthAbsolute(2);

		if (this->selectionStart) {
			RECT bounds = getSelectionBounds(*this->selectionStart, this->selectionEnd);
			bounds.right += 1;
			bounds.bottom += 1;
			renderer.drawRect(bounds);
		}
	}
}


void LevelEditor::drawPalette(Renderer& renderer)
{
	int tileCount = this->tilePalette.size();

	for (int i = 0; i < tileCount; i++)
	{
		int x = i % PALETTE_WIDTH;
		int y = i / PALETTE_WIDTH;

		Vector2 delta = { PALETTE_MARGIN + x * (PALETTE_MARGIN + 1), PALETTE_MARGIN + y * (PALETTE_MARGIN + 1) };
		renderer.offset(delta);

		renderer.setFillColor(0, 0, 0);
		renderer.setLineStyle(LINE_NONE);

		renderTileFromID(renderer, this->tilePalette[i]);

		renderer.offset(-delta);
	}
}

const TileID* LevelEditor::selectPaletteTile(Vector2i mouse)
{
	Vector2 cursor = Vector2(mouse) / TILE_SIZE;
	cursor.x -= this->level.getWidth();

	double size = 1 + PALETTE_MARGIN;

	int x = floor(cursor.x / size);
	int y = floor(cursor.y / size);

	cursor.x -= x * size;
	cursor.y -= y * size;

	if (PALETTE_MARGIN < cursor.x && cursor.x < PALETTE_MARGIN + 1 &&
		PALETTE_MARGIN < cursor.y && cursor.y < PALETTE_MARGIN + 1)
	{
		int index = x + y * PALETTE_WIDTH;

		if (index < this->tilePalette.size())
		{
			return &this->tilePalette[index];
		}
	}
	return nullptr;
}


void LevelEditor::keyPressed(int key)
{
	if (key == VK_ESCAPE) {
		this->close();
	}
	if (key == 'G')
	{
		this->grid = !this->grid;
	}
}

void LevelEditor::mouseMoved(int x, int y)
{
	Vector2i tileCoord = { x / TILE_SIZE, y / TILE_SIZE };
	tileCoord.x = clamp(tileCoord.x, 0, this->level.getWidth() - 1);
	tileCoord.y = clamp(tileCoord.y, 0, this->level.getHeight() - 1);

	if (this->selectionStart) {
		this->selectionEnd = tileCoord;
	}
}

void LevelEditor::mousePressed(MouseButton button, int x, int y)
{
	if (button == MOUSE_LEFT || button == MOUSE_RIGHT) {
		if (x < LEVEL_SIZE_PIXELS.cx && y < LEVEL_SIZE_PIXELS.cy) {
			delete this->selectionStart;
			this->selectionStart = new Vector2i(x / TILE_SIZE, y / TILE_SIZE);
			this->selectionEnd = *this->selectionStart;
		}
		else if (button == MOUSE_LEFT)
		{
			if (const TileID* tile = this->selectPaletteTile({ x, y }))
			{
				this->currentTile = tile;
			}
		}
	}
}

void LevelEditor::mouseReleased(MouseButton button, int x, int y)
{
	if (button == MOUSE_LEFT && this->selectionStart) {
		this->selectionEnd = { x / TILE_SIZE, y / TILE_SIZE };

		std::vector<Vector2i> coords = getSelectionCoords(*this->selectionStart, this->selectionEnd);
		int coordCount = coords.size();
		for (int i = 0; i < coordCount; i++) {
			TileID tileId = *this->currentTile;

			if (tileId == TILE_PLAYER_START)
			{
				level.setNinjaSpawn(coords[i]);
			}

			this->level.setTile(coords[i], createTileFromID(tileId));
		}

		delete this->selectionStart;
		this->selectionStart = nullptr;
	}

	if (button == MOUSE_RIGHT && this->selectionStart) {
		this->selectionEnd = { x / TILE_SIZE, y / TILE_SIZE };

		std::vector<Vector2i> coords = getSelectionCoords(*this->selectionStart, this->selectionEnd);
		int coordCount = coords.size();
		for (int i = 0; i < coordCount; i++) {
			this->level.setTile(coords[i], nullptr);
		}

		delete this->selectionStart;
		this->selectionStart = nullptr;
	}
}

RECT getSelectionBounds(Vector2i start, Vector2i end)
{
	RECT bounds;
	if (start.x < end.x) { bounds.left = start.x; bounds.right = end.x; }
	else { bounds.left = end.x; bounds.right = start.x; }

	if (start.y < end.y) { bounds.top = start.y; bounds.bottom = end.y; }
	else { bounds.top = end.y; bounds.bottom = start.y; }

	return bounds;
}

std::vector<Vector2i> getSelectionCoords(Vector2i start, Vector2i end)
{
	RECT bounds = getSelectionBounds(start, end);

	std::vector<Vector2i> coords;

	for (int x = bounds.left; x <= bounds.right; x++)
	{
		for (int y = bounds.top; y <= bounds.bottom; y++)
		{
			coords.push_back({ x, y });
		}
	}

	return coords;
}
