#include "LevelSelector.h"
#include "Interpolate.h"

LevelSelector::LevelSelector(App* parent) :
	App(parent),
	playButton(Vector2(1280 - TILE_SIZE * TILE_MARGIN - 80, 720 - TILE_SIZE*TILE_MARGIN - 80), 60),
	difficulty(NORMAL)
{
	this->createThumbnails({
		"levels/level0.lvl",
		"levels/level1.lvl",
		"levels/level2.lvl"
	});

	this->setWindowSize(1280, 720); 

	this->createInformationPane();
}

LevelSelector::~LevelSelector()
{
	delete this->selectedLevel;
}

void LevelSelector::update(float deltaTime)
{
	this->playButton.update(deltaTime);
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
				this->difficulty = Difficulty(i);
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
		this->difficulty = Difficulty((int(this->difficulty) + 1) % 3);
	}
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

void LevelSelector::createThumbnails(const std::vector<std::string>& paths)
{
	double width = LEVEL_SIZE.cx * TILE_SIZE;
	double height = LEVEL_SIZE.cy * TILE_SIZE;
	double margin = TILE_SIZE * TILE_MARGIN;

	int pathCount = paths.size();
	for (int i = 0; i < pathCount; i++)
	{
		Level level = Level(paths[i], NORMAL);

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

void LevelSelector::drawLevels(Renderer& renderer)
{
	int levelCount = this->levels.size();
	for (int i = 0; i < levelCount; i++)
	{
		LevelThumbnail& thumbnail = this->levels[i];

		if (this->selectedLevel && *this->selectedLevel == i) {
			renderer.setFillColor(200, 200, 200);
		}
		else if (this->highlightedLevel && *this->highlightedLevel == i) {
			renderer.setFillColor(125, 125, 125);
		}
		else {
			renderer.setFillColor(100, 100, 100);
		}

		renderer.fillRect(thumbnail.container);


		Vector2 delta(thumbnail.container.left, thumbnail.container.top);

		if (!thumbnail.levelBitmap)
		{
			thumbnail.levelBitmap = new Bitmap(this->renderLevelThumbnail(renderer, thumbnail.level));
		}

		renderer.drawBitmapTransparent(
			*thumbnail.levelBitmap,
			63, 174, 12,
			thumbnail.container.left, thumbnail.container.top
		);
	}
}

void LevelSelector::drawLevelInformation(Renderer& renderer)
{
	if(this->selectedLevel)
	{
		Vector2 offset = renderer.getOffset();

		SIZE windowSize = this->getWindowSize();
		double width = windowSize.cx - offset.x - TILE_SIZE * TILE_MARGIN;
		double height = windowSize.cy - offset.y - TILE_SIZE * TILE_MARGIN;

		int grey = 25;

		renderer.setFillColor(grey, grey, grey);
		renderer.fillRect(0, 0, width, height);

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


		// Rita highscores


		// Rita spel/start
		renderer.setLineStyle(LINE_NONE);
		this->playButton.render(renderer);
		renderer.offset(offset);
	}
}

void LevelSelector::changeSelected(int delta)
{
	if (this->selectedLevel)
	{
		*this->selectedLevel = clamp(
			*this->selectedLevel + delta,
			0, int(this->levels.size() - 1)
		);
	} else
	{
		this->selectedLevel = new int(delta > 0 ? 0 : this->levels.size() - 1);
	}
}

void LevelSelector::playLevel(int levelIndex)
{
	if (0 <= levelIndex && levelIndex < this->levels.size())
	{
		this->levels[levelIndex].level.setDifficulty(this->difficulty);
		this->addChild(new NinjaGame(this, this->levels[levelIndex].level));
	}
}
