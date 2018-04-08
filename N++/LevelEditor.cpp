#include "LevelEditor.h"

LevelEditor::LevelEditor(App* parent) :
	App(parent), level(LEVEL_SIZE.cx, LEVEL_SIZE.cy), grid(true)
{
	this->setWindowSize(LEVEL_SIZE.cx * TILE_SIZE, LEVEL_SIZE.cy * TILE_SIZE);

	this->levelBitmap = this->createCompatibleBitmap(this->getWindowSize());
	this->currentTile = new SquareTile();
}

void LevelEditor::update(float deltaTime)
{

}

void LevelEditor::draw(Renderer& renderer)
{
	renderer.scale(TILE_SIZE);

	renderer.setFillColor(0, 255, 255);
	renderer.clear();

	this->drawLevel(renderer);

	if (grid) this->drawGrid(renderer);

	this->drawSelection(renderer);
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

	renderer.drawBitmap(this->levelBitmap, 0, 0, this->levelBitmap.getWidth(), this->levelBitmap.getHeight(), 0, 0);
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
		renderer.setFillColor(50, 200, 200);
		renderer.setColor(0, 0, 255);
		renderer.setLineStyle(LINE_SOLID);
		renderer.setLineWidthAbsolute(2);

		this->currentTile->render(renderer);

		if (this->selectionStart) {
			std::vector<Vector2i> coords = getSelectionCoords(*this->selectionStart, this->selectionEnd);
			int coordCount = coords.size();
			for (int i = 0; i < coordCount; i++)
			{
				Vector2i coord = coords[i];

				this->currentTile->setPosition(coord);
				this->currentTile->render(renderer);
			}
		}
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
	if (this->currentTile) this->currentTile->setPosition(tileCoord);

	if(this->selectionStart) {
		this->selectionEnd = tileCoord;
	}
}

void LevelEditor::mousePressed(MouseButton button, int x, int y)
{
	if (button == MOUSE_LEFT) {
		this->selectionStart = new Vector2i(x / TILE_SIZE, y / TILE_SIZE);
		this->selectionEnd = *this->selectionStart;
	}
}

void LevelEditor::mouseReleased(MouseButton button, int x, int y)
{
	if (button == MOUSE_LEFT && this->selectionStart) {
		this->selectionEnd = { x / TILE_SIZE, y / TILE_SIZE };

		std::vector<Vector2i> coords = getSelectionCoords(*this->selectionStart, this->selectionEnd);
		int coordCount = coords.size();
		for (int i = 0; i < coordCount; i++)
		{
			Vector2i coord = coords[i];

			if (this->level.getTile(coord)) {
				this->level.setTile(coord, nullptr);
			}
			else {
				this->level.setTile(coord, this->currentTile->clone());
			}
		}

		delete this->selectionStart;
		this->selectionStart = nullptr;
	}
}

std::vector<Vector2i> getSelectionCoords(Vector2i start, Vector2i end)
{
	int minX, maxX, minY, maxY;

	if (start.x < end.x) { minX = start.x; maxX = end.x; } else { minX = end.x; maxX = start.x; }
	if (start.y < end.y) { minY = start.y; maxY = end.y; } else { minY = end.y; maxY = start.y; }

	std::vector<Vector2i> coords;

	for (int x = minX; x <= maxX; x++)
	{
		for (int y = minY; y <= maxY; y++)
		{
			coords.push_back({ x, y });
		}
	}

	return coords;
}
