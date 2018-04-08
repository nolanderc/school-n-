#include "NinjaGame.h"

NinjaGame::NinjaGame(App* parent) :
	App(parent), level("levels/level0.lvl")
{
	this->setWindowTitle("N++");
	this->setWindowSize(level.getWidth() * TILE_SIZE, level.getHeight() * TILE_SIZE);

	levelBitmap = this->createCompatibleBitmap(this->getWindowSize());
	this->renderLevel = true;
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
		Renderer levelRenderer = renderer.createBitmapRenderer(this->levelBitmap);
		levelRenderer.scale(TILE_SIZE);

		levelRenderer.setFillColor(255, 255, 0);
		levelRenderer.clear();

		levelRenderer.setFillColor(0, 0, 0);
		level.renderStatic(levelRenderer);

		DeleteDC(levelRenderer.releaseDC());
		
		this->renderLevel = false;
	}

	renderer.drawBitmap(this->levelBitmap, 0, 0, this->levelBitmap.getWidth(), this->levelBitmap.getHeight(), 0, 0);


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


