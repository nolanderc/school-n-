#include "LevelEditor.h"


LevelEditor::LevelEditor(App* parent) :
	App(parent), level(LEVEL_SIZE.cx, LEVEL_SIZE.cy), grid(true)
{
	this->setWindowSize(LEVEL_SIZE_PIXELS.cx + PALETTE_WIDTH_PIXELS, LEVEL_SIZE_PIXELS.cy);

	this->levelBitmap = this->createCompatibleBitmap(LEVEL_SIZE_PIXELS);
	this->currentTile = new SquareTile();

	this->createPalette();
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
	deleteVectorElements(this->tilePalette);

	this->tilePalette.push_back(new SquareTile());
	this->tilePalette.push_back(new ExitTile());

	for (int i = 0; i < 4; i++) {
		this->tilePalette.push_back(new WedgeTile(i));
	}

	this->tilePalette.push_back(new ActiveMine());
	this->tilePalette.push_back(new InactiveMine());

	this->tilePalette.push_back(new CoinTile());
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

		if (this->currentTileCoord) {
			this->currentTile->render(renderer);
		}

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

		double margin = (double(PALETTE_WIDTH_PIXELS) / TILE_SIZE - 2.0) / 3.0;

		this->tilePalette[i]->setPosition({ 0 });

		Vector2 delta = { margin + x * (margin + 1), margin + y * (margin + 1) };
		renderer.offset(delta);
	
		renderer.setFillColor(0, 0, 0);
		renderer.setLineStyle(LINE_NONE);
		this->tilePalette[i]->render(renderer);

		renderer.offset(-delta);
	}
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
	if (this->currentTile) this->currentTile->setPosition(tileCoord);

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
	}
}

void LevelEditor::mouseReleased(MouseButton button, int x, int y)
{
	if (button == MOUSE_LEFT && this->selectionStart) {
		this->selectionEnd = { x / TILE_SIZE, y / TILE_SIZE };

		std::vector<Vector2i> coords = getSelectionCoords(*this->selectionStart, this->selectionEnd);
		int coordCount = coords.size();
		for (int i = 0; i < coordCount; i++) {
			this->level.setTile(coords[i], this->currentTile->clone());
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
