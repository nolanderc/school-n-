#include "LevelSelector.h"
#include "Interpolate.h"

LevelSelector::LevelSelector(App* parent) :
	App(parent)
{
	this->createThumbnails({
		"levels/level0.lvl",
		"levels/customLevel.lvl"
	});

	this->setWindowSize(1280, 720);
}

LevelSelector::~LevelSelector()
{
	delete this->selectedLevel;
}

void LevelSelector::update(float deltaTime)
{
}

void LevelSelector::draw(Renderer& renderer)
{
	renderer.setFillColor(50, 50, 150);
	renderer.clear();


	int levelCount = this->levels.size();
	for (int i = 0; i < levelCount; i++)
	{
		LevelThumbnail& thumbnail = this->levels[i];

		if (this->selectedLevel && *this->selectedLevel == i) {
			renderer.setFillColor(255, 255, 100);
		} else {
			renderer.setFillColor(100, 100, 100);
		}

		renderer.fillRect(thumbnail.container);
		

		Vector2 delta(thumbnail.container.left, thumbnail.container.top);
		renderer.offset(delta);
		renderer.scale(TILE_SIZE);

		thumbnail.level.renderStatic(renderer);
		thumbnail.level.renderDynamic(renderer);

		renderer.scale(1.0 / TILE_SIZE);
		renderer.offset(-delta);

		if (this->selectedLevel && *this->selectedLevel == i)
		{
			renderer.setFillColor(0, 255, 0);
			renderer.fillRect(thumbnail.playContainer);
		}
	}
}

void LevelSelector::mouseMoved(int x, int y)
{
	if (this->selectedLevel)
	{
		delete this->selectedLevel;
		this->selectedLevel = nullptr;
	}

	int levelCount = this->levels.size();
	for (int i = 0; i < levelCount; i++)
	{
		LevelThumbnail& thumbnail = this->levels[i];
		if (thumbnail.container.contains(Vector2(x, y)))
		{
			delete this->selectedLevel;
			this->selectedLevel = new int(i);
		}
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
			if(thumbnail.playContainer.contains(Vector2(x, y)))
			{
				if(this->selectedLevel && *this->selectedLevel == i)
				{
					this->addChild(new NinjaGame(this, this->levels[*this->selectedLevel].level));
					delete this->selectedLevel;
					this->selectedLevel = nullptr;
				}
			}
		}
	}
}

void LevelSelector::keyPressed(int key)
{
	if (key == VK_ESCAPE)
	{
		this->close();
	}
}

void LevelSelector::createThumbnails(const std::vector<std::string>& paths)
{
	Vector2 offset = { TILE_SIZE * TILE_MARGIN };

	int pathCount = paths.size();
	for (int i = 0; i < pathCount; i++)
	{
		Level level = Level(paths[i], NORMAL);

		double width = level.getWidth() * TILE_SIZE;
		double height = level.getHeight() * TILE_SIZE;

		Vector2 center = offset + Vector2(width / 2, height / 2);

		LevelThumbnail thumbnail = {
			level,
			
			BoundingBox(
				offset.x, offset.x + width, 
				offset.y, offset.y + height
			),

			BoundingBox(
				center.x - 40, center.x + 40,
				center.y - 40, center.y + 40
			),
		};

		offset.y += height + TILE_MARGIN * TILE_SIZE;

		this->levels.push_back(thumbnail);
	}
}
