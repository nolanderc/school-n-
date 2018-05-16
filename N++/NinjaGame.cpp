#include "NinjaGame.h"


NinjaGame::NinjaGame(App* parent, Level level, VictoryCallback* victoryCallback, std::string playerName) : 
	App(parent), level(level), victoryCallback(victoryCallback), alive(true), playerName(playerName), navBar(0, 32, this)
{
	this->level.setVictoryCallback(this);

	this->setWindowSize(level.getWidth() * TILE_SIZE, level.getHeight() * TILE_SIZE + ENERGY_BAR_HEIGHT_PIXELS + this->navBar.getHeight());

	this->navBar.setWidth(this->getWindowSize().x);

	levelBitmap = this->createCompatibleBitmap(this->getWindowSize());

	this->createNavBar();
}

NinjaGame::~NinjaGame()
{
	delete this->levelBitmap;
}

void NinjaGame::update(float dt)
{
	static double accumulator = 0;
	accumulator += dt;
	const double deltaTime = 1.0 / 240;

	while (accumulator > deltaTime) {
		accumulator -= deltaTime;
		if (accumulator > 1) {
			accumulator = 0;
		}
		
		if (this->isKeyDown(KEY_LEFT) || this->isKeyDown(KEY_A)) { this->level.moveNinja(NINJA_LEFT); }
		if (this->isKeyDown(KEY_RIGHT) || this->isKeyDown(KEY_D)) { this->level.moveNinja(NINJA_RIGHT); }

		this->level.update(deltaTime);
	}

	if (this->victory) {
		this->victoryTime += deltaTime;
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

	Color background(50);

	if (this->level.needsRerender()) {
		Renderer* levelRenderer = renderer.createBitmapRenderer(this->levelBitmap);
		levelRenderer->setFillColor(background);
		levelRenderer->clear();

		levelRenderer->scale(TILE_SIZE);

		levelRenderer->setFillColor(0, 0, 0);
		level.renderStatic(*levelRenderer);

		delete levelRenderer;
	}

	renderer.drawBitmap(this->levelBitmap, 0, ENERGY_BAR_HEIGHT_PIXELS, this->levelBitmap->getWidth(), this->levelBitmap->getHeight(), 0, 0);

	renderer.offset({ 0, double(ENERGY_BAR_HEIGHT_PIXELS) / TILE_SIZE });

	renderer.setColor(100, 100, 100);
	renderer.setLineWidth(1.0 / TILE_SIZE);

	level.renderDynamic(renderer, background);

	renderer.scale(1.0 / TILE_SIZE);
	renderer.offset(-renderer.getOffset());

	this->renderVictoryScreen(renderer);


	renderer.setTextColor(Color(150));
	renderer.offset(Vector2(0, this->getWindowSize().y - this->navBar.getHeight()));
	this->navBar.render(renderer, Color(0));
}

void NinjaGame::onLevelFail()
{
	if (this->victoryCallback) this->victoryCallback->onLevelFail();

	this->alive = false;
	this->createNavBar();
}


void NinjaGame::keyPressed(KeyCode key)
{
	this->navBar.keyPressed(key);

	if (key == KEY_UP || key == KEY_W) {
		this->level.moveNinja(NINJA_JUMP);
	}
	if (key == KEY_DOWN || key == KEY_S) {
		this->level.moveNinja(NINJA_DROP);
	}


	if (this->victory) {
		if (this->playerName.size() < 20 &&
			('A' <= key && key <= 'Z')) {
			this->playerName += key;
		}

		if (key == KEY_BACKSPACE && !this->playerName.empty()) {
			this->playerName.pop_back();
		}
	}
}

void NinjaGame::keyReleased(KeyCode key)
{
	if (key == KEY_UP) {
		this->level.moveNinja(NINJA_CANCEL_JUMP);
	}
}


void NinjaGame::renderVictoryScreen(Renderer & renderer)
{
	if (this->victory) {
		Vector2i windowSize = this->getWindowSize();

		Color background(125);
		renderer.setFillColor(background);
		renderer.setTextBackgroundColor(background);
		renderer.setLineStyle(LINE_NONE);

		double width = 400;
		double height = 200;

		Vector2 center( 
			windowSize.x / 2.0, 
			easeInOut(normalize(this->victoryTime, 0, 1.0), -height / 2, windowSize.y / 2.0)
		);


		BoundingBox box(
			center.x - width / 2.0, center.x + width / 2.0,
			center.y - height / 2.0, center.y + height / 2.0
		);

		renderer.fillRect(box);

		// Rita rubriken
		BoundingBox completeBox = box;
		completeBox.bottom = box.top + 48;

		renderer.setTextColor(Color(255));
		renderer.drawTextCentered("Level Complete!", completeBox);

		
		// Rita tid och mynt
		BoundingBox scoreBox = box;
		scoreBox.left += width / 4;
		scoreBox.right -= width / 4;
		scoreBox.bottom = box.bottom - 32;
		scoreBox.top = scoreBox.bottom - 32;

		renderer.drawTextLeftAligned("Coins:", scoreBox);
		renderer.drawTextRightAligned(std::to_string(this->victory->coins), scoreBox);

		scoreBox.top -= 32;
		scoreBox.bottom -= 32;

		renderer.drawTextLeftAligned("Time:", scoreBox);
		renderer.drawTextRightAligned(formatTime(this->victory->time), scoreBox);



		// Rita spelarens namn
		BoundingBox nameBox = box;
		nameBox.left += width / 4;
		nameBox.right -= width / 4;
		nameBox.top = box.top + 48;
		nameBox.bottom = nameBox.top + 64;

		renderer.drawTextLeftAligned("Name: " + this->playerName, nameBox);
	}
}

void NinjaGame::createNavBar()
{
	this->navBar.clear();

	this->navigation.back = this->navBar.addButton("Back", KEY_ESCAPE);

	if (this->victory || !this->alive)
		this->navigation.reset = this->navBar.addButton("Retry", KEY_TAB);
	else 
		this->navigation.reset = -1;

	if (this->victory) 
		this->navigation.nextLevel = this->navBar.addButton("Continue to Next Level", KEY_ENTER);
	else 
		this->navigation.nextLevel = -1;
}

void NinjaGame::onLevelComplete(double time, int coins, std::string name)
{
	delete this->victory;
	this->victory = new Victory(time, coins);

	this->createNavBar();
}

void NinjaGame::navigate(int id)
{
	if (this->victory) {
		if (this->victoryCallback) this->victoryCallback->onLevelComplete(this->victory->time, this->victory->coins, this->playerName);
	}


	if (id == this->navigation.back) {
		this->close();
	}

	if (id == this->navigation.reset) {
		this->level.retry();

		delete this->victory;
		this->victory = nullptr;
		this->victoryTime = 0;

		this->alive = true;

		this->createNavBar();
	}

	if (id == this->navigation.nextLevel) {
		this->close(NINJA_GAME_NEXT_LEVEL);
	}
}
