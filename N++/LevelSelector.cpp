#include "LevelSelector.h"
#include "Interpolate.h"

LevelSelector::LevelThumbnail::~LevelThumbnail()
{
	delete this->levelBitmap;
}

LevelSelector::LevelSelector(App* parent) :
	App(parent),
	completedLevelCount(0),
	levelList("levels/levels.list"),
	scrollAmount(0),
	scrollTarget(0),
	recentHighscore(nullptr),
	difficulty(NORMAL),
	playButton(Vector2(1280 - TILE_SIZE * TILE_MARGIN - 80, 800 - TILE_SIZE*TILE_MARGIN - 80), 60)
{
	this->createThumbnails(this->levelList);
	this->checkCompletedLevels();


	this->setWindowSize(1280, 800); 
	this->maxScroll = this->levels.empty() ? 0 : this->levels.back().container.bottom - this->getWindowSize().cy + TILE_MARGIN * TILE_SIZE;

	this->createInformationPane();
}

LevelSelector::~LevelSelector()
{
	delete this->selectedLevel;
	delete this->highlightedLevel;
	delete this->recentHighscore;
}

void LevelSelector::onLevelComplete(double time, int coins)
{
	Score score;
	score.time = time;
	score.coins = coins;
	
	delete this->recentHighscore;
	this->recentHighscore = new Highscore(
		*this->selectedLevel,
		this->difficulty,
		this->levelList.addNewScore(*this->selectedLevel, this->difficulty, score)
	);

	this->checkCompletedLevels();
}

void LevelSelector::update(float deltaTime)
{
	this->playButton.update(deltaTime);

	double scrollDelta = this->scrollTarget - this->scrollAmount;
	this->scroll(abs(scrollDelta) < 1 ? scrollDelta : scrollDelta * deltaTime * 10);
}

void LevelSelector::draw(Renderer& renderer)
{
	renderer.setFillColor(20, 20, 20);
	renderer.clear();


	this->drawLevels(renderer);

	Vector2 delta(TILE_SIZE * (3 * TILE_MARGIN + 2 * LEVEL_SIZE.cx), TILE_SIZE * TILE_MARGIN);
	renderer.offset(delta);
	this->drawLevelInformation(renderer);
	renderer.offset(delta);
}

void LevelSelector::mouseMoved(int x, int y)
{
	if (this->highlightedLevel)
	{
		delete this->highlightedLevel;
		this->highlightedLevel = nullptr;
	}

	int levelCount = this->levels.size();
	for (int i = 0; i < levelCount; i++)
	{
		LevelThumbnail& thumbnail = this->levels[i];
		if (thumbnail.container.contains(Vector2(x, y)))
		{
			delete this->highlightedLevel;
			this->highlightedLevel = new int(i);
		}
	}


	if (this->selectedLevel && this->playButton.contains(Vector2(x, y))) {
		this->playButton.setHighlight(true);
	} else {
		this->playButton.setHighlight(false);
		this->playButton.setSelected(false);
	}
}

void LevelSelector::mousePressed(MouseButton button, int x, int y)
{
	if (button == MOUSE_LEFT)
	{
		// Nivåer
		int levelCount = this->levels.size();
		for (int i = 0; i < levelCount; i++)
		{
			LevelThumbnail& thumbnail = this->levels[i];
			if(thumbnail.container.contains(Vector2(x, y)))
			{
				delete this->selectedLevel;
				this->selectedLevel = new int(i);
			}
		}


		// Spela-knappen
		if (this->selectedLevel && this->playButton.contains(Vector2(x, y))) {
			this->playButton.setSelected(true);
		}

		// Svårihetsgrad
		int difficulties = this->difficultyContainers.size();
		for (int i = 0; i < difficulties; ++i)
		{
			if (this->difficultyContainers[i].contains(Vector2(x, y)))
			{
				this->changeDifficulty(Difficulty(i));
			}
		}
	}
}

void LevelSelector::mouseReleased(MouseButton button, int x, int y)
{
	if (button == MOUSE_LEFT)
	{
		if (this->selectedLevel && this->playButton.contains(Vector2(x, y))) {
			this->playButton.setSelected(false);

			this->playLevel(*this->selectedLevel);
		}
	}
}

void LevelSelector::mouseScrolled(int wheelDelta, int x, int y)
{
	this->addScroll(wheelDelta * -100);
}

void LevelSelector::keyPressed(int key)
{
	if (key == VK_ESCAPE)
	{
		this->close();
	}

	if (key == VK_RETURN || key == VK_SPACE)
	{
		if (this->selectedLevel)
		{
			this->playLevel(*this->selectedLevel);
		}
	}

	if(key == VK_UP || key == 'W')
	{
		this->changeSelected(-2);
	}

	if (key == VK_DOWN || key == 'S')
	{
		this->changeSelected(2);
	}

	if (key == VK_LEFT || key == 'A')
	{
		this->changeSelected(-1);
	}

	if (key == VK_RIGHT || key == 'D')
	{
		this->changeSelected(1);
	}

	if (key == VK_TAB)
	{
		this->changeDifficulty(Difficulty((int(this->difficulty) + 1) % 3));
	}
}

void LevelSelector::scroll(double delta)
{
	this->scrollAmount += delta;

	if (delta)
	{
		int levelCount = this->levels.size();
		for (int i = 0; i < levelCount; i++) {
			LevelThumbnail& level = this->levels[i];

			level.container.top -= delta;
			level.container.bottom -= delta;
		}
	}
}

void LevelSelector::addScroll(double delta)
{
	double scrollDelta = this->scrollTarget - this->scrollAmount;

	if (sign(scrollDelta) == sign(delta)) {
		this->scrollTarget += delta;
	} else {
		this->scrollTarget = this->scrollAmount + delta;
	}

	this->scrollTarget = clamp(
		this->scrollTarget,
		0.0,
		this->maxScroll
	);
}

void LevelSelector::changeDifficulty(Difficulty difficulty)
{
	this->difficulty = difficulty;
	this->checkCompletedLevels();
	if (this->selectedLevel) this->changeSelected(0);
}

void LevelSelector::checkCompletedLevels()
{
	// Beräkna för alla svårighetsgrader högre än denna
	int maxCompleted = 0;

	// Hard låser upp alla.
	// Normal låser upp Easy och Normal.
	// Easy låser bara upp sig själv.
	for (int d = int(this->difficulty); d <= int(HARD); d++) {
		int completed = 0;

		int levelCount = this->levelList.size();
		for (int i = 0; i < levelCount; i++) {
			std::vector<Score> scores = this->levelList.getScores(i, Difficulty(d));

			if (!scores.empty()) {
				completed++;
			} else {
				break;
			}
		}

		if (completed > maxCompleted) maxCompleted = completed;
	}

	this->completedLevelCount = maxCompleted;
}

void LevelSelector::createInformationPane()
{
	Vector2 topLeft(TILE_SIZE * (3 * TILE_MARGIN + 2 * LEVEL_SIZE.cx), TILE_SIZE * TILE_MARGIN);
	SIZE windowSize = this->getWindowSize();
	double width = windowSize.cx - topLeft.x - TILE_SIZE * TILE_MARGIN;
	double height = windowSize.cy - topLeft.y - TILE_SIZE * TILE_MARGIN;

	for (int i = 0; i < 3; ++i)
	{
		this->difficultyContainers.push_back(BoundingBox(
			topLeft.x + i * width / 3, topLeft.x + (i + 1) * width / 3,
			topLeft.y, topLeft.y + 32
		));
	}
}

void LevelSelector::createThumbnails(const LevelList& list)
{
	double width = LEVEL_SIZE.cx * TILE_SIZE;
	double height = LEVEL_SIZE.cy * TILE_SIZE;
	double margin = TILE_SIZE * TILE_MARGIN;

	int levelCount = list.size();
	for (int i = 0; i < levelCount; i++)
	{
		Level level = list.getLevel(i);

		int x = i % 2;
		int y = i / 2;

		Vector2 offset(
			(width + margin) * x + margin,
			(height + margin) * y + margin
		);

		LevelThumbnail thumbnail = {
			level,

			nullptr,
			
			BoundingBox(
				offset.x, offset.x + width, 
				offset.y, offset.y + height
			),
		};

		this->levels.push_back(thumbnail);
	}
}

Bitmap LevelSelector::renderLevelThumbnail(Renderer& renderer, Level& level)
{
	int width = TILE_SIZE * level.getWidth();
	int height = TILE_SIZE * level.getHeight();

	Bitmap target = this->createCompatibleBitmap({width, height});
	Renderer bitmapRenderer = renderer.createBitmapRenderer(target);

	bitmapRenderer.setFillColor(63, 174, 12);
	bitmapRenderer.clear();

	bitmapRenderer.scale(TILE_SIZE);

	bitmapRenderer.setFillColor(0, 0, 0);

	level.renderStatic(bitmapRenderer);
	level.renderDynamic(bitmapRenderer);

	return target;
}

void LevelSelector::drawLock(Renderer& renderer, const BoundingBox& container)
{
	double width = container.right - container.left;
	double height = container.bottom - container.top;

	double size = (width < height ? width : height) / 2;

	Vector2 center(
		(container.left + container.right) / 2,
		(container.top + container.bottom) / 2 - size / 4
	);


	renderer.fillRect(
		center.x - size / 2, center.y,
		size, size * 0.8
	);


	renderer.setLineStyle(LINE_SOLID);
	renderer.setLineWidth(size / 8);
	renderer.drawCircle(
		center.x, center.y, size * 0.4
	);

	renderer.setLineWidth(size / 8);
	renderer.setColor(50, 50, 50);
	renderer.drawLine(
		center.x, center.y + size * 0.3,
		center.x, center.y + size * 0.5
	);

	renderer.setLineStyle(LINE_NONE);
	renderer.setFillColor(50, 50, 50);
	renderer.fillCircle(
		center.x, center.y + size * 0.3,
		size / 8
	);

}

void LevelSelector::drawLevels(Renderer& renderer)
{
	int levelCount = this->levels.size();
	for (int i = 0; i < levelCount; i++)
	{
		LevelThumbnail& thumbnail = this->levels[i];

		if (this->completedLevelCount + 2 > i)
		{
			if (this->selectedLevel && *this->selectedLevel == i) {
				renderer.setFillColor(200, 200, 200);
			}
			else if (this->highlightedLevel && *this->highlightedLevel == i) {
				renderer.setFillColor(125, 125, 125);
			}
			else {
				renderer.setFillColor(50, 50, 50);
			}

			BoundingBox box = thumbnail.container;
			box.left += TILE_SIZE / 2;
			box.top += TILE_SIZE / 2;
			box.right -= TILE_SIZE / 2;
			box.bottom -= TILE_SIZE / 2;


			renderer.fillRect(box);


			if (!thumbnail.levelBitmap) {
				thumbnail.levelBitmap = new Bitmap(this->renderLevelThumbnail(renderer, thumbnail.level));
			}

			renderer.drawBitmapTransparent(
				*thumbnail.levelBitmap,
				63, 174, 12,
				thumbnail.container.left, thumbnail.container.top
			);
		} else {
			renderer.setFillColor(50, 50, 50);
			renderer.fillRect(thumbnail.container);

			renderer.setFillColor(125, 125, 125);
			renderer.setColor(125, 125, 125);
			this->drawLock(renderer, thumbnail.container);
		}
	}
}

std::string LevelSelector::formatTime(double time)
{
	int tenths = floor(fmod(time, 1) * 10);
	int hundredths = floor(fmod(time, 0.1) * 100);
	int minutes = int(time) / 60;
	int seconds = floor(time) - minutes * 60;

	std::stringstream ss;
	ss << minutes << " : " << seconds << "." << tenths << hundredths;

	return ss.str();
}

void LevelSelector::drawLevelInformation(Renderer& renderer)
{
	Vector2 offset = renderer.getOffset();

	SIZE windowSize = this->getWindowSize();
	double margin = TILE_SIZE * TILE_MARGIN;
	double width = windowSize.cx - offset.x - margin;
	double height = windowSize.cy - offset.y - margin;

	int grey = 25;

	renderer.setFillColor(grey, grey, grey);
	renderer.fillRect(0, 0, width, height);


	// Rita svårighetens förändringar
	switch (this->difficulty) { 
	case EASY: renderer.setTextColor(200, 200, 200); break;
	case NORMAL: renderer.setTextColor(200, 200, 50); break;
	case HARD: renderer.setTextColor(200, 50, 50); break;
	default: ;
	}
	renderer.setTextBackgroundColor(grey, grey, grey);


	renderer.drawTextLeftAligned(
		std::string("Energy Loss: ") + (this->difficulty == EASY ? "1x" : this->difficulty == NORMAL ? "1.25x" : "1.5x"),
		BoundingBox(
			margin, width - margin,
			height - 120, height - 80
		)
	);

	renderer.drawTextLeftAligned(
		std::string("Enemy Speed: ") + (this->difficulty == EASY ? "1x" : this->difficulty == NORMAL ? "1.16x" : "1.33x"),
		BoundingBox(
			margin, width - margin,
			height - 80, height - 40
		)
	);


	renderer.offset(-offset);

	// Rita svårighet
	int difficulties = this->difficultyContainers.size();
	for (int i = 0; i < difficulties; ++i)
	{
		if (i == this->difficulty) {
			renderer.setFillColor(grey, grey, grey);
			renderer.setTextBackgroundColor(grey, grey, grey);

			renderer.setTextColor(255, 255, 255);
		} else {
			renderer.setFillColor(grey / 2, grey / 2, grey / 2);
			renderer.setTextBackgroundColor(grey / 2, grey / 2, grey / 2);

			renderer.setTextColor(150, 150, 150);
		}

		renderer.fillRect(this->difficultyContainers[i]);
			
		std::string text;
		switch (i)
		{
		case EASY: text = "Easy"; break;
		case NORMAL: text = "Normal"; break;
		case HARD: text = "Hard"; break;
		default: text = "Nan";
		}

		renderer.drawTextCentered(text, this->difficultyContainers[i]);
	}


	if (this->selectedLevel)
	{
		// Rita spel/start
		renderer.setLineStyle(LINE_NONE);
		this->playButton.render(renderer);
		renderer.offset(offset);


		// Rita highscores
		renderer.setTextBackgroundColor(grey, grey, grey);

		renderer.setTextColor(255, 255, 255);
		BoundingBox box;
		box.left = 32;
		box.right = width - box.left;
		box.top = 48;
		box.bottom = box.top + 32;

		renderer.drawTextLeftAligned("Time", box);
		renderer.drawTextCentered("Coins", box);
		renderer.drawTextRightAligned("Points", box);

		std::vector<Score> highscores = this->levelList.getScores(*this->selectedLevel, this->difficulty);
		int scoreCount = highscores.size();
		for (int i = 0; i < scoreCount && i < 10; ++i)
		{
			Score score = highscores[i];

			BoundingBox rect;
			rect.left = 32;
			rect.right = width - rect.left;
			rect.top = box.bottom + 16 + 32 * i;
			rect.bottom = rect.top + 32;

			if (this->recentHighscore && 
				this->recentHighscore->level == *this->selectedLevel &&
				this->recentHighscore->difficulty == this->difficulty &&
				this->recentHighscore->score == i) 
			{
				renderer.setTextColor(255, 255, 0);
			} else if (i == 0) {
				renderer.setTextColor(255, 255, 255);
			} else {
				renderer.setTextColor(150, 150, 150);
			}

			renderer.drawTextLeftAligned(formatTime(score.time), rect);
			renderer.drawTextCentered(std::to_string(score.coins), rect);
			renderer.drawTextRightAligned(std::to_string(int(score.getValue() * 10)), rect);
		}
	}
}

void LevelSelector::changeSelected(int delta)
{
	int levelCount = this->levels.size();
	int unlockedCount = clamp(this->completedLevelCount + 2, 0, levelCount);

	int maxLevel = (levelCount < unlockedCount ? levelCount : unlockedCount) - 1;

	if (this->selectedLevel)
	{
		*this->selectedLevel = clamp(
			*this->selectedLevel + delta,
			0, maxLevel
		);
	} else
	{
		this->selectedLevel = new int(delta > 0 ? 0 : maxLevel);
	}


	// Centrera den markerade nivån på skärmen
	BoundingBox box = this->levels[*this->selectedLevel].container;

	double center = (box.top + box.bottom) / 2;
	this->addScroll(center - this->getWindowSize().cy / 2.0);
}

void LevelSelector::playLevel(int levelIndex)
{
	if (0 <= levelIndex && levelIndex < this->levels.size())
	{
		this->levels[levelIndex].level.setDifficulty(this->difficulty);
		this->addChild(new NinjaGame(this, this->levels[levelIndex].level, this));
	}
}
