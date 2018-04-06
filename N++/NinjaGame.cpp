#include "NinjaGame.h"

#include <fstream>


NinjaGame::NinjaGame() : 
	App(3 * TILE_SIZE, 3 * TILE_SIZE, "N++"), 
	level(0, 0)
{
	currentTile = new CoinTile();

	levelBitmap = this->createCompatibleBitmap(this->getWindowSize());
	this->renderLevel = true;

	std::ifstream file("levels/level0.lvl");
	if (file.is_open())
	{
		level = Level(file);
		this->setWindowSize(level.getWidth() * TILE_SIZE, level.getHeight() * TILE_SIZE);
	} else
	{
		MessageBox(NULL, "Failed to open 'level0.lvl'", "Error", MB_OK | MB_ICONERROR);
	}
}

void NinjaGame::update(float dt)
{
	updateFrameCounter(dt);

	static double accumulator = 0;
	accumulator += dt;
	const double deltaTime = 1.0 / 1000;

	while (accumulator > deltaTime) {
		accumulator -= deltaTime;
		if (accumulator > 1) {
			accumulator = 0;
		}

		if (this->isKeyDown(VK_LEFT)) { this->level.moveNinja(NINJA_LEFT); }
		if (this->isKeyDown(VK_RIGHT)) { this->level.moveNinja(NINJA_RIGHT); }

		this->level.update(deltaTime);
	}
}

void NinjaGame::draw(Renderer & renderer)
{
	renderer.setFillColor(0, 255, 255);
	renderer.clear();

	renderer.scale(TILE_SIZE);

	
	if (this->renderLevel || this->level.needsRerender()) {
		HDC dc = renderer.createCompatibleDC();
		HBITMAP old = this->levelBitmap.selectInto(dc);

		Renderer levelRenderer = Renderer(dc, this->levelBitmap.getSize());
		levelRenderer.scale(TILE_SIZE);

		levelRenderer.setFillColor(255, 255, 0);
		levelRenderer.clear();

		levelRenderer.setFillColor(0, 0, 0);

		level.renderStatic(levelRenderer);

		
		SelectObject(dc, old);
		DeleteDC(dc);

		this->renderLevel = false;
	}

	renderer.drawBitmap(this->levelBitmap, 0, 0, this->levelBitmap.getWidth(), this->levelBitmap.getHeight(), 0, 0);


	if (selectedTile && currentTile) {
		renderer.setLineStyle(LineStyle::LINE_SOLID);
		renderer.setColor(255, 0, 0);
		renderer.setLineWidth(2.0 / TILE_SIZE);

		if (level.getTile(*selectedTile)) {
			renderer.drawLine(selectedTile->x, selectedTile->y, selectedTile->x + 1, selectedTile->y + 1);
			renderer.drawLine(selectedTile->x, selectedTile->y + 1, selectedTile->x + 1, selectedTile->y);
		}
		else {
			currentTile->setPosition(*selectedTile);
			currentTile->render(renderer);
		}
	}

	renderer.setColor(100, 100, 100);
	renderer.setLineWidth(2.0 / TILE_SIZE);

	level.renderDynamic(renderer);
}

void NinjaGame::sizeChanged(int width, int height)
{
	levelBitmap = this->createCompatibleBitmap({ width, height });
	this->renderLevel = true;
}

void NinjaGame::keyPressed(int key)
{
	if (key == VK_UP) {
		this->level.moveNinja(NINJA_JUMP);
	}
	if (key == VK_DOWN) {
		this->level.moveNinja(NINJA_DROP);
	}

	if (key == ' ') {
		this->level.retry();
	}

	if (key == 'R') {
		this->level.reset();
	}

	if (key == VK_ESCAPE) {
		this->close();
	}
}

void NinjaGame::keyReleased(int key)
{
	if (key == VK_UP) {
		this->level.moveNinja(NINJA_CANCEL_JUMP);
	}
}

void NinjaGame::mousePressed(MouseButton button, int x, int y)
{
	if (button == MOUSE_LEFT) {
		if (selectedTile) {
			const Tile* old = level.getTile(*selectedTile);

			if (old) {
				level.setTile(*selectedTile, nullptr);
			}
			else {
				level.setTile(*selectedTile, currentTile->clone());
			}
		}

		this->renderLevel = true;
	}
}

void NinjaGame::mouseMoved(int x, int y)
{
	int tileX = x / TILE_SIZE;
	int tileY = y / TILE_SIZE;

	if (selectedTile) {
		selectedTile->x = tileX;
		selectedTile->y = tileY;
	}
	else {
		selectedTile = new Vector2i(tileX, tileY);
	}
}


#include <sstream>

void NinjaGame::updateFrameCounter(double deltaTime)
{
	static double elapsedTime = 0;
	static int frames = 0;

	elapsedTime += deltaTime;
	frames += 1;
	if (elapsedTime > 1) {
		int fps = round(frames / elapsedTime);
		elapsedTime = 0;
		frames = 0;

		std::stringstream ss;
		ss << "N++ @ " << fps;

		this->setWindowTitle(ss.str());
	}
}


