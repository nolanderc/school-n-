#include "LevelSelector.h"
#include "Interpolate.h"


const Vector2i CONTENT_SIZE(1600, 800);



LevelSelector::LevelThumbnail::~LevelThumbnail()
{
	delete this->levelBitmap;
}


LevelSelector::LevelSelector(App* parent) :
	App(parent),
	completedLevelCount(0),
	levelList("levels/levels.list"),
	recentHighscore(nullptr),
	difficulty(NORMAL),
	playButton(Vector2(CONTENT_SIZE.x - TILE_SIZE * TILE_MARGIN - 80, CONTENT_SIZE.y - TILE_SIZE*TILE_MARGIN - 80), 60),
	navBar(CONTENT_SIZE.x, 32, this),
	scrollHelper(0.0)
{
	this->createThumbnails(this->levelList);
	this->checkCompletedLevels();


	this->setWindowSize(CONTENT_SIZE.x, CONTENT_SIZE.y + this->navBar.getHeight());
	double maxScroll = this->levels.empty() ? 0 : this->levels.back().container.bottom - CONTENT_SIZE.y + TILE_MARGIN * TILE_SIZE;

	this->scrollHelper.setMaxScroll(maxScroll);

	this->createInformationPane();


	this->navigation.back = navBar.addButton("Back", KEY_ESCAPE);
	this->navigation.changeDifficulty = navBar.addButton("Change Difficulty", KEY_TAB);
	this->navigation.playLevel = navBar.addButton("Play Selected Level", KEY_ENTER);
}

LevelSelector::~LevelSelector()
{
	delete this->selectedLevel;
	delete this->highlightedLevel;
	delete this->recentHighscore;
}

void LevelSelector::onLevelComplete(double time, int coins, std::string name)
{
	Score score;
	score.time = time;
	score.coins = coins;
	score.name = name;
	
	delete this->recentHighscore;
	this->recentHighscore = new Highscore(
		*this->selectedLevel,
		this->difficulty,
		this->levelList.addNewScore(*this->selectedLevel, this->difficulty, score)
	);

	this->checkCompletedLevels();
}

void LevelSelector::navigate(int id)
{
	if (id == this->navigation.back) {
		this->close();
	}

	if (id == this->navigation.changeDifficulty) {
		this->changeDifficulty(Difficulty((int(this->difficulty) + 1) % 3));
	}

	if (id == this->navigation.playLevel) {
		if (this->selectedLevel) {
			this->playLevel(*this->selectedLevel);
		}
	}
}

void LevelSelector::update(float deltaTime)
{
	this->playButton.update(deltaTime);


	double scrollBefore = this->scrollHelper.getDistance();
	this->scrollHelper.update(deltaTime * 10);
	double scrollAfter = this->scrollHelper.getDistance();

	double delta = scrollAfter - scrollBefore;

	// Flytta alla nivåer nedåt
	if (delta) {
		int levelCount = this->levels.size();
		for (int i = 0; i < levelCount; i++) {
			LevelThumbnail& level = this->levels[i];

			level.container.top -= delta;
			level.container.bottom -= delta;

			level.starContainer.top -= delta;
			level.starContainer.bottom -= delta;
		}

		Vector2i mouse = this->getMousePosition();
		this->mouseMoved(mouse.x, mouse.y);
	}
}

void LevelSelector::draw(Renderer& renderer)
{
	renderer.setFillColor(20, 20, 20);
	renderer.clear();


	this->drawLevels(renderer);

	Vector2 delta(TILE_SIZE * (3 * TILE_MARGIN + 2 * (LEVEL_SIZE.x + STAR_SIZE)), TILE_SIZE * TILE_MARGIN);
	renderer.offset(delta);
	this->drawLevelInformation(renderer);
	renderer.offset(-delta);


	if (!this->tooltip.empty())
	{
		renderer.setTextColor(Color(200));
		renderer.setTextBackgroundColor(Color(75));

		Vector2i mouse = this->getMousePosition();

		std::string line;
		int lineNumber = 0;
		std::stringstream stream;
		stream << this->tooltip;

		while (std::getline(stream, line))
		{
			double y = mouse.y - 10 + lineNumber * 16;

			renderer.drawTextLeftAligned(line, BoundingBox(mouse.x + 10, mouse.x + 10, y, y));
			lineNumber += 1;
		}
	}


	renderer.setTextColor(Color(255));
	renderer.offset(Vector2(0, this->getWindowSize().y - this->navBar.getHeight()));
	this->navBar.render(renderer, Color(0));
}

void LevelSelector::mouseMoved(int x, int y)
{
	if (this->highlightedLevel)
	{
		delete this->highlightedLevel;
		this->highlightedLevel = nullptr;
	}

	this->checkTooltip(Vector2(x, y));


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
	this->scrollHelper.addScroll(wheelDelta * -100);
}

void LevelSelector::keyPressed(KeyCode key)
{
	this->navBar.keyPressed(key);
	
	if(key == KEY_UP || key == KEY_W)
	{
		this->changeSelected(-2);
	}

	if (key == KEY_DOWN || key == KEY_S)
	{
		this->changeSelected(2);
	}

	if (key == KEY_LEFT || key == KEY_A)
	{
		this->changeSelected(-1);
	}

	if (key == KEY_RIGHT || key == KEY_D)
	{
		this->changeSelected(1);
	}
}

void LevelSelector::childClosed(int exitCode)
{
	if (exitCode == NINJA_GAME_NEXT_LEVEL) {
		if (this->selectedLevel && *this->selectedLevel == this->levels.size() - 1) {
			this->alert("Congratulations!", "I didn't want to make a final victory screen, so this is all you get! Congratulations, you finished the game!");
		} else {
			this->changeSelected(1);
			if (this->selectedLevel) {
				this->playLevel(*this->selectedLevel);
			}
		}
	}
}

void LevelSelector::changeDifficulty(Difficulty difficulty)
{
	this->difficulty = difficulty;
	this->checkCompletedLevels();
	if (this->selectedLevel) this->changeSelected(0);
	
	Vector2i mouse = this->getMousePosition(); 
	this->mouseMoved(mouse.x, mouse.y);
}

int LevelSelector::getLevelStarCount(int level)
{
	int stars = 3;
	for (int d = int(HARD); d >= int(EASY); d--)
	{
		std::vector<Score> scores = this->levelList.getScores(level, Difficulty(d));

		if (!scores.empty()) return stars;
		stars--;
	}

	return 0;
}

void LevelSelector::checkCompletedLevels()
{
	// Beräkna för alla svårighetsgrader högre än denna
	this->completedLevelCount = 0;

	// Hard låser upp alla.
	// Normal låser upp Easy och Normal.
	// Easy låser bara upp sig själv.
	int levelCount = this->levelList.size();
	for (int i = 0; i < levelCount; i++) {
		int stars = this->getLevelStarCount(i);

		if (stars > int(this->difficulty)) {
			this->completedLevelCount++;
		}

		this->levels[i].starCount = stars;
	}
}

void LevelSelector::checkTooltip(Vector2 mouse)
{
	this->tooltip.clear();

	this->checkLevelTooltip(mouse);

	this->checkDifficultyTooltip(mouse);
}

void LevelSelector::checkLevelTooltip(Vector2 mouse)
{
	int levelCount = this->levels.size();
	for (int i = 0; i < levelCount; i++)
	{
		LevelThumbnail& thumbnail = this->levels[i];

		// För låsta nivåer
		if (thumbnail.container.contains(mouse))
		{
			delete this->highlightedLevel;
			this->highlightedLevel = new int(i);

			if (this->completedLevelCount + 2 <= i)
			{
				int necessaryStars = i - this->completedLevelCount - 1;
				std::string difficulty = this->difficulty == EASY ? "Easy" : this->difficulty == NORMAL ? "Normal" : "Hard";
				this->tooltip = "You need " + std::to_string(necessaryStars) + " additional `" + difficulty + "`-star" + (necessaryStars > 1 ? "s" : "") + " to unlock";
			}
		}


		// För stjärnor
		if (thumbnail.starContainer.contains(mouse))
		{
			double p = 1.0 - normalize(mouse.y, thumbnail.starContainer.top, thumbnail.starContainer.bottom);

			if (p > 2.0 / 3.0) this->tooltip = "Earned by completing level on `Hard` difficulty";
			else if (p > 1.0 / 3.0) this->tooltip = "Earned by completing level on `Normal` difficulty or higher";
			else if (p > 0.0 / 3.0) this->tooltip = "Earned by completing level on `Easy` difficulty or higher";
		}
	}
}

void LevelSelector::checkDifficultyTooltip(Vector2 mouse)
{
	int difficultyCount = this->difficultyContainers.size();
	for (int i = 0; i < difficultyCount; i++)
	{
		if (this->difficultyContainers[i].contains(mouse))
		{
			Difficulty difficulty = Difficulty(i);

			this->tooltip = std::string("Energy Loss: ") + (difficulty == EASY ? "1x" : difficulty == NORMAL ? "1.25x" : "1.5x") + "\n" +
				std::string("Enemy Speed: ") + (difficulty == EASY ? "1x" : difficulty == NORMAL ? "1.16x" : "1.33x");
		}
	}
}

void LevelSelector::createInformationPane()
{
	Vector2 topLeft(TILE_SIZE * (3 * TILE_MARGIN + 2 * (LEVEL_SIZE.x + STAR_SIZE)), TILE_SIZE * TILE_MARGIN);
	double width = CONTENT_SIZE.x - topLeft.x - TILE_SIZE * TILE_MARGIN;
	double height = CONTENT_SIZE.y - topLeft.y - TILE_SIZE * TILE_MARGIN;

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
	double width = LEVEL_SIZE.x * TILE_SIZE;
	double height = LEVEL_SIZE.y * TILE_SIZE;

	double starWidth = (LEVEL_SIZE.x + STAR_SIZE) * TILE_SIZE;

	double margin = TILE_SIZE * TILE_MARGIN;

	int levelCount = list.size();
	for (int i = 0; i < levelCount; i++)
	{
		Level level = list.getLevel(i);

		int x = i % 2;
		int y = i / 2;

		Vector2 offset(
			(starWidth + margin) * x + margin,
			(height + margin) * y + margin
		);

		LevelThumbnail thumbnail = {
			level,

			nullptr,
			
			BoundingBox(
				offset.x, offset.x + width, 
				offset.y, offset.y + height
			),

			BoundingBox(
				offset.x + width, offset.x + starWidth,
				offset.y, offset.y + height
			),

			this->getLevelStarCount(i)
		};

		this->levels.push_back(thumbnail);
	}
}

Bitmap* LevelSelector::renderLevelThumbnail(Renderer& renderer, Level& level)
{
	int width = TILE_SIZE * level.getWidth();
	int height = TILE_SIZE * level.getHeight();

	Bitmap* target = this->createCompatibleBitmap({width, height});
	Renderer* bitmapRenderer = renderer.createBitmapRenderer(target);

	bitmapRenderer->setFillColor(BACKGROUND_COLOR);
	bitmapRenderer->clear();

	bitmapRenderer->scale(TILE_SIZE);

	bitmapRenderer->setFillColor(0, 0, 0);

	level.renderStatic(*bitmapRenderer);
	level.renderDynamic(*bitmapRenderer, BACKGROUND_COLOR);

	delete bitmapRenderer;

	return target;
}

void LevelSelector::drawLock(Renderer & renderer, const BoundingBox & container, Color backgroundColor)
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
	renderer.setColor(backgroundColor);
	renderer.drawLine(
		center.x, center.y + size * 0.3,
		center.x, center.y + size * 0.5
	);

	renderer.setLineStyle(LINE_NONE);
	renderer.setFillColor(backgroundColor);
	renderer.fillCircle(
		center.x, center.y + size * 0.3,
		size / 8
	);

}

void LevelSelector::fillStar(Renderer& renderer, Vector2 center, double radius)
{
	std::vector<Vector2> points;

	double deltaAngle = 4 * PI / 5.0;

	for (int i = 0; i < 5; i++)
	{
		Vector2 direction(
			radius * cos(deltaAngle * i - PI / 2),
			radius * sin(deltaAngle * i - PI / 2)
		);

		points.push_back(center + direction);
	}

	renderer.fillPolygon(points); 
}

void LevelSelector::drawLevels(Renderer& renderer)
{
	int levelCount = this->levels.size();
	for (int i = 0; i < levelCount; i++)
	{
		LevelThumbnail& thumbnail = this->levels[i];


		// Rita stjärnor och lås
		renderer.setFillColor(0, 0, 0);
		renderer.fillRect(thumbnail.starContainer);

		double x = (thumbnail.starContainer.left + thumbnail.starContainer.right) / 2;

		double width = thumbnail.starContainer.right - thumbnail.starContainer.left;
		double height = thumbnail.starContainer.bottom - thumbnail.starContainer.top;

		for (int i = 0; i < 3; i++) {

			double y = thumbnail.starContainer.bottom - (i + 0.5) * height / 3;

			if (i < thumbnail.starCount) {
				renderer.setFillColor(200, 200, 0);
				renderer.setColor(200, 200, 0);
			} else {
				renderer.setFillColor(50, 50, 50);
				renderer.setColor(50, 50, 50);
			}

			double size = (width < height ? width : height) * 0.35;

			if (i > int(this->difficulty))
			{
				this->drawLock(renderer, BoundingBox(
					x - size, x + size,
					y - size, y + size
				), Color(0));
			}
			else
			{
				renderer.setLineStyle(LINE_NONE);
				this->fillStar(renderer, { x, y }, size);
			}

		}

		
		// Rita nivån
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
				thumbnail.levelBitmap = this->renderLevelThumbnail(renderer, thumbnail.level);
			}

			renderer.drawBitmapTransparent(
				thumbnail.levelBitmap,
				BACKGROUND_COLOR,
				thumbnail.container.left, thumbnail.container.top
			);
		} else {
			renderer.setFillColor(50, 50, 50);
			renderer.fillRect(thumbnail.container);

			renderer.setFillColor(125, 125, 125);
			renderer.setColor(125, 125, 125);
			this->drawLock(renderer, thumbnail.container, Color(50));
		}
	}
}


void LevelSelector::drawLevelInformation(Renderer& renderer)
{
	Vector2 offset = renderer.getOffset();

	double margin = TILE_SIZE * TILE_MARGIN;
	double width = CONTENT_SIZE.x - offset.x - margin;
	double height = CONTENT_SIZE.y - offset.y - margin;

	int grey = 25;

	renderer.setFillColor(grey, grey, grey);
	renderer.fillRect(0, 0, width, height);

	// Rita svårighet
	renderer.offset(-offset);
	this->drawDifficultyContainers(renderer, grey);
	renderer.offset(offset);

	if (this->selectedLevel)
	{
		// Rita spel/start
		renderer.setLineStyle(LINE_NONE);
		renderer.offset(-offset);
		this->playButton.render(renderer);
		renderer.offset(offset);

		BoundingBox highscoreBox;
		highscoreBox.left = 32;
		highscoreBox.right = width - highscoreBox.left;
		highscoreBox.top = 48;
		highscoreBox.bottom = height - 200;

		// Rita highscores
		renderer.setTextBackgroundColor(grey, grey, grey);
		this->drawHighscores(renderer, highscoreBox);
	}
}

void LevelSelector::drawDifficultyContainers(Renderer& renderer, int grey)
{
	int difficulties = this->difficultyContainers.size();
	for (int i = 0; i < difficulties; ++i)
	{
		if (i == this->difficulty) {
			renderer.setFillColor(grey, grey, grey);
			renderer.setTextBackgroundColor(grey, grey, grey);

			renderer.setTextColor(255, 255, 255);
		}
		else {
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
}

void LevelSelector::drawHighscoreHeader(Renderer& renderer, BoundingBox box)
{
	BoundingBox nameRect = box;
	nameRect.left = box.left;
	nameRect.right = nameRect.left + (box.right - box.left) / 2.5;
	nameRect.top = box.top;
	nameRect.bottom = nameRect.top + 32;

	BoundingBox scoreRect = nameRect;
	scoreRect.left = nameRect.right;
	scoreRect.right = box.right;

	renderer.drawTextLeftAligned("Name", nameRect);

	renderer.drawTextLeftAligned("Time", scoreRect);
	renderer.drawTextCentered("Coins", scoreRect);
	renderer.drawTextRightAligned("Points", scoreRect);
}

void LevelSelector::drawHighscores(Renderer& renderer, BoundingBox box)
{
	if (this->selectedLevel) {
		// Rita tabellhuvud
		renderer.setTextColor(Color(255));
		this->drawHighscoreHeader(renderer, box);

		box.top += 48;

		std::vector<Score> highscores = this->levelList.getScores(*this->selectedLevel, this->difficulty);
		int scoreCount = highscores.size();
		for (int i = 0; i < scoreCount && i < 10; ++i)
		{
			Score score = highscores[i];

			if (this->recentHighscore &&
				this->recentHighscore->level == *this->selectedLevel &&
				this->recentHighscore->difficulty == this->difficulty &&
				this->recentHighscore->score == i) {
				// Det senast tillagda rekordet
				renderer.setTextColor(255, 255, 0);
			}
			else if (i == 0) {
				// Den högsta poängen
				renderer.setTextColor(255, 255, 255);
			}
			else {
				// Resterande poäng
				renderer.setTextColor(150, 150, 150);
			}

			BoundingBox nameRect = box;
			nameRect.left = box.left;
			nameRect.right = nameRect.left + (box.right - box.left) / 2.5;
			nameRect.top = box.top + 32 * i;
			nameRect.bottom = nameRect.top + 32;

			BoundingBox scoreRect = nameRect;
			scoreRect.left = nameRect.right;
			scoreRect.right = box.right;

			// Rita namnet, om det finns
			renderer.drawTextLeftAligned(
				score.name.empty() ? "---" : score.name,
				nameRect
			);

			// Tid, mynt och poäng
			renderer.drawTextLeftAligned(formatTime(score.time), scoreRect);
			renderer.drawTextCentered(std::to_string(score.coins), scoreRect);
			renderer.drawTextRightAligned(std::to_string(int(score.getValue() * 10)), scoreRect);
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
		this->selectedLevel = new int(0);
	}


	// Centrera den markerade nivån på skärmen
	BoundingBox box = this->levels[*this->selectedLevel].container;

	double center = (box.top + box.bottom) / 2;
	this->scrollHelper.addScroll(center - this->getWindowSize().y / 2.0);
}

void LevelSelector::playLevel(int levelIndex)
{
	if (0 <= levelIndex && levelIndex < this->levels.size())
	{
		this->levels[levelIndex].level.setDifficulty(this->difficulty);

		this->addChild(new NinjaGame(this, this->levels[levelIndex].level, this, this->levelList.getPreviousName()));
	}
}
