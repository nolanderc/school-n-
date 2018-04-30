#include "LevelEditor.h"


LevelEditor::LevelEditor(App* parent, std::string path) :
	App(parent), path(path), level(path, HARD), grid(true), tilePalette(2, PALETTE_WIDTH_PIXELS / double(TILE_SIZE))
{
	if (this->level.getWidth() == 0)
	{
		this->level = Level(LEVEL_SIZE.x, LEVEL_SIZE.y, HARD);
	}

	this->setWindowSize(LEVEL_SIZE_PIXELS.x + PALETTE_WIDTH_PIXELS, LEVEL_SIZE_PIXELS.y);

	this->levelBitmap = this->createCompatibleBitmap(LEVEL_SIZE_PIXELS);


	Vector2 buttonPosition = Vector2(
		LEVEL_SIZE_PIXELS.x + BUTTONS_HEIGHT_PIXELS / 2,
		BUTTONS_HEIGHT_PIXELS / 2
	);

	this->buttons.push_back(new SaveButton(buttonPosition, BUTTONS_HEIGHT_PIXELS / 2));

	buttonPosition.x += BUTTONS_HEIGHT_PIXELS;
	this->buttons.push_back(new RunButton(buttonPosition, BUTTONS_HEIGHT_PIXELS / 2));


	this->createLevelBorder();
}

void LevelEditor::createLevelBorder()
{
	for (int x = 0; x < this->level.getWidth(); x++)
	{
		this->level.setTile({ x, 0 }, new SquareTile());
		this->level.setTile({ x, this->level.getHeight() - 1 }, new SquareTile());
	}

	for (int y = 0; y < this->level.getHeight(); y++)
	{
		this->level.setTile({ 0, y }, new SquareTile());
		this->level.setTile({ this->level.getWidth() - 1, y }, new SquareTile());
	}
}

void LevelEditor::update(float deltaTime)
{
	int count = this->buttons.size();
	for (int i = 0; i < count; i++)
	{
		this->buttons[i]->update(deltaTime);
	}
}

void LevelEditor::draw(Renderer& renderer)
{
	renderer.setFillColor(100, 100, 100);
	renderer.clear();

	this->drawButtons(renderer);
	
	renderer.scale(TILE_SIZE);

	this->drawLevel(renderer);

	if (grid) this->drawGrid(renderer);

	this->drawSelection(renderer);


	renderer.offset({ double(this->level.getWidth()), double(BUTTONS_HEIGHT_PIXELS) / double(TILE_SIZE) });
	this->tilePalette.draw(renderer);
}

void LevelEditor::closed()
{
	this->level.save(this->path);
}


void LevelEditor::drawLevel(Renderer& renderer)
{
	if (this->level.needsRerender())
	{
		Renderer* levelRenderer = renderer.createBitmapRenderer(this->levelBitmap);

		levelRenderer->setFillColor(50, 50, 50);
		levelRenderer->clear();

		levelRenderer->scale(TILE_SIZE);

		levelRenderer->setFillColor(0, 0, 0);
		level.renderStatic(*levelRenderer);
	}

	// Rita nivån
	renderer.drawBitmap(this->levelBitmap, 0, 0, this->levelBitmap->getWidth(), this->levelBitmap->getHeight(), 0, 0);

	renderer.setFillColor(50, 50, 50);
	renderer.clear();

	this->level.renderStatic(renderer);
	this->level.renderDynamic(renderer);

	// Rita en ram runt nivån
	renderer.setColor(0, 0, 0);
	renderer.setLineStyle(LINE_SOLID);
	renderer.setLineWidthAbsolute(4);
	renderer.drawRect(0, 0, this->level.getWidth(), this->level.getHeight());
}

void LevelEditor::drawGrid(Renderer& renderer)
{
	renderer.setColor(60, 60, 60);
	renderer.setLineStyle(LINE_SOLID);
	renderer.setLineWidthAbsolute(1);

	for (int x = 1; x < this->level.getWidth(); x++)
	{
		renderer.drawLine(x, 0, x, this->level.getHeight());
	}

	for (int y = 1; y < this->level.getHeight(); y++)
	{
		renderer.drawLine(0, y, this->level.getWidth(), y);
	}
}

void LevelEditor::drawSelection(Renderer& renderer)
{
	if (this->selectionStart) {
		renderer.setFillColor(200, 200, 20);
		renderer.setColor(0, 0, 255);
		renderer.setLineStyle(LINE_SOLID);
		renderer.setLineWidthAbsolute(2);

		BoundingBoxi bounds = getSelectionBounds(*this->selectionStart, this->selectionEnd);
		bounds.right += 1;
		bounds.bottom += 1;
		renderer.drawRect(bounds);
	}
}

void LevelEditor::drawButtons(Renderer& renderer)
{
	int count = this->buttons.size();
	for (int i = 0; i < count; i++)
	{
		this->buttons[i]->render(renderer);
	}
}


void LevelEditor::selectPaletteTile(Vector2i mouse)
{
	mouse.x -= this->level.getWidth() * TILE_SIZE;
	mouse.y -= BUTTONS_HEIGHT_PIXELS;

	if (mouse.x >= 0 && mouse.y >= 0)
	{
		Vector2i coord = (mouse * PALETTE_WIDTH) / PALETTE_WIDTH_PIXELS;

		this->tilePalette.selectTile(coord);
	}
}

void LevelEditor::setSelectionTile(Tile* tile)
{
	if (this->selectionStart)
	{
		std::vector<Vector2i> coords = getSelectionCoords(*this->selectionStart, this->selectionEnd);
		int coordCount = coords.size();
		for (int i = 0; i < coordCount; i++) {
			Vector2i coord = coords[i];

			// Kan inte ändra väggarna
			if (coord.x > 0 && coord.x < this->level.getWidth() - 1 &&
				coord.y > 0 && coord.y < this->level.getHeight() - 1)
			{
				this->level.setTile(coord, tile == nullptr ? nullptr : tile->clone());
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

	if (key == 'R' || key == VK_F5)
	{
		this->level.save(this->path);

		// Starta ett spel innuti redigeraren
		this->addChild(new NinjaGame(this, this->level));
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


	int count = this->buttons.size();
	for (int i = 0; i < count; i++)
	{
		MenuButton* button = this->buttons[i];
		if (button->contains({double(x), double(y)}))
		{
			button->setHighlight(true);
		}
		else
		{
			button->setHighlight(false);
			button->setSelected(false);
		}
	}
}


void LevelEditor::mousePressed(MouseButton button, int x, int y)
{
	if (button == MOUSE_LEFT || button == MOUSE_RIGHT) {
		if (x < LEVEL_SIZE_PIXELS.x && y < LEVEL_SIZE_PIXELS.y) {
			delete this->selectionStart;
			this->selectionStart = new Vector2i(x / TILE_SIZE, y / TILE_SIZE);
			this->selectionEnd = *this->selectionStart;
		}
		else if (button == MOUSE_LEFT)
		{
			this->selectPaletteTile({ x, y });
		}
	}

	if (button == MOUSE_LEFT)
	{
		int buttonCount = this->buttons.size();
		for (int i = 0; i < buttonCount; i++)
		{
			MenuButton* menuButton = this->buttons[i];
			if (menuButton->contains(Vector2(x, y))) {
				menuButton->setSelected(true);
			}
		}
	}

	if(button == MOUSE_MIDDLE)
	{
		Vector2i tileCoord = { x / TILE_SIZE, y / TILE_SIZE };
		tileCoord.x = clamp(tileCoord.x, 0, this->level.getWidth() - 1);
		tileCoord.y = clamp(tileCoord.y, 0, this->level.getHeight() - 1);

		const Tile* tile = this->level.getTile(tileCoord);
		if (tile)
		{
			this->tilePalette.setCurrentTile(createTileIdFromName(tile->getFormattedName()));
		} else if (tileCoord == this->level.getNinjaSpawn())
		{
			this->tilePalette.setCurrentTile(TILE_PLAYER_START);
		}
	}
}

void LevelEditor::mouseReleased(MouseButton button, int x, int y)
{
	if ((button == MOUSE_LEFT || button == MOUSE_RIGHT) && this->selectionStart) {
		this->selectionEnd = { x / TILE_SIZE, y / TILE_SIZE };
		TileID tileId = this->tilePalette.getCurrentTile();

		Tile* tile = button == MOUSE_LEFT ? createTileFromID(tileId) : nullptr;
		this->setSelectionTile(tile);
		delete tile;

		if (button == MOUSE_LEFT && tileId == TILE_PLAYER_START) {
			Vector2i coord = this->selectionEnd;

			// Kan inte ändra väggarna
			if (coord.x > 0 && coord.x < this->level.getWidth() - 1 &&
				coord.y > 0 && coord.y < this->level.getHeight() - 1)
			{
				level.setNinjaSpawn(coord);
			}
		}

		delete this->selectionStart;
		this->selectionStart = nullptr;
	}


	if (button == MOUSE_LEFT)
	{
		int buttonCount = this->buttons.size();
		for (int i = 0; i < buttonCount; i++)
		{
			MenuButton* menuButton = this->buttons[i];
			if (menuButton->contains(Vector2(x, y))) {
				menuButton->setHighlight(false);
				menuButton->setSelected(false);

				switch (menuButton->getID())
				{
				case EDITOR_BUTTON_SAVE:
					this->level.save(this->path);
					break;

				case EDITOR_BUTTON_RUN:
					this->level.save(this->path);

					// Starta ett spel innuti redigeraren
					this->addChild(new NinjaGame(this, this->level));
					break;

				default: break;
				}
			}
		}
	}
}

BoundingBoxi getSelectionBounds(Vector2i start, Vector2i end)
{
	BoundingBoxi bounds;
	if (start.x < end.x) { bounds.left = start.x; bounds.right = end.x; }
	else { bounds.left = end.x; bounds.right = start.x; }

	if (start.y < end.y) { bounds.top = start.y; bounds.bottom = end.y; }
	else { bounds.top = end.y; bounds.bottom = start.y; }

	return bounds;
}

std::vector<Vector2i> getSelectionCoords(Vector2i start, Vector2i end)
{
	BoundingBoxi bounds = getSelectionBounds(start, end);

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
