#include "NinjaGame.h"


NinjaGame::NinjaGame(App* parent, Level level, VictoryCallback* victoryCallback) : 
	App(parent), level(level), gameCallback(victoryCallback)
{
	this->level.setVictoryCallback(this);

	this->setWindowTitle("N++");
	this->setWindowSize(level.getWidth() * TILE_SIZE, level.getHeight() * TILE_SIZE + ENERGY_BAR_HEIGHT_PIXELS);

	levelBitmap = this->createCompatibleBitmap(this->getWindowSize());
	this->renderLevel = true;
}

NinjaGame::~NinjaGame()
{
	delete this->levelBitmap;
}

void NinjaGame::update(float dt)
{
	updateFrameCounter(dt);

	static double accumulator = 0;
	accumulator += dt;
	const double deltaTime = 1.0 / 240;

	while (accumulator > deltaTime) {
		accumulator -= deltaTime;
		if (accumulator > 1) {
			accumulator = 0;
		}
		
		if (this->isKeyDown(VK_LEFT) || this->isKeyDown('A')) { this->level.moveNinja(NINJA_LEFT); }
		if (this->isKeyDown(VK_RIGHT) || this->isKeyDown('D')) { this->level.moveNinja(NINJA_RIGHT); }

		this->level.update(deltaTime);
	}
}

void NinjaGame::draw(Renderer & renderer)
{
	renderer.setFillColor(0, 0, 0);
	renderer.clear();

	renderer.setFillColor(255, 255, 0);

	double width = this->getWindowSize().x * this->level.getEnergyPercentage();

	renderer.fillRect(0, 0, width, ENERGY_BAR_HEIGHT_PIXELS);

	renderer.scale(TILE_SIZE);

	if (this->renderLevel || this->level.needsRerender()) {
		Renderer* levelRenderer = renderer.createBitmapRenderer(this->levelBitmap);
		levelRenderer->setFillColor(50, 50, 50);
		levelRenderer->clear();

		levelRenderer->scale(TILE_SIZE);

		levelRenderer->setFillColor(0, 0, 0);
		level.renderStatic(*levelRenderer);

		this->renderLevel = false;
	}

	renderer.drawBitmap(this->levelBitmap, 0, ENERGY_BAR_HEIGHT_PIXELS, this->levelBitmap->getWidth(), this->levelBitmap->getHeight(), 0, 0);

	renderer.offset({ 0, double(ENERGY_BAR_HEIGHT_PIXELS) / TILE_SIZE });

	renderer.setColor(100, 100, 100);
	renderer.setLineWidth(1.0 / TILE_SIZE);

	level.renderDynamic(renderer);

	renderer.scale(1.0 / TILE_SIZE);

	this->renderVictoryScreen(renderer);
}


void NinjaGame::sizeChanged(int width, int height)
{
	levelBitmap = this->createCompatibleBitmap({ width, height });
	this->renderLevel = true;
}

void NinjaGame::keyPressed(int key)
{
	if (key == VK_UP || key == 'W') {
		this->level.moveNinja(NINJA_JUMP);
	}
	if (key == VK_DOWN || key == 'S') {
		this->level.moveNinja(NINJA_DROP);
	}

	if (key == VK_SPACE) {
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

void NinjaGame::renderVictoryScreen(Renderer & renderer)
{
	if (this->victory) {
		Vector2i windowSize = this->getWindowSize();

		renderer.setFillColor(Color(125));
		renderer.setLineStyle(LINE_NONE);

		Vector2 center(windowSize.x / 2.0, windowSize.y / 2.0);

		double width = 500;
		double height = 300;

		renderer.fillRect(
			center.x - width / 2.0, center.y - height / 2.0,
			width, height
		);
	}
}

void NinjaGame::onLevelComplete(double time, int coins)
{
	if (this->gameCallback) this->gameCallback->onLevelComplete(time, coins);

	delete this->victory;
	this->victory = new Victory(time, coins);
}


