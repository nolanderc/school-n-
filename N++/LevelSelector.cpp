#include "LevelSelector.h"
#include "Interpolate.h"

LevelSelector::LevelSelector(App* parent) :
	App(parent)
{
	this->setWindowSize(1280, 720);

	this->levels.push_back(Level("levels/level0.lvl", NORMAL));
	this->levels.push_back(Level("levels/customLevel.lvl", NORMAL));
}

void LevelSelector::update(float deltaTime)
{
}

void LevelSelector::draw(Renderer& renderer)
{
	renderer.setFillColor(50, 50, 150);
	renderer.clear();


	renderer.scale(TILE_SIZE);
	renderer.offset({ TILE_MARGIN });

	int levelCount = this->levels.size();
	for (int i = 0; i < levelCount; i++)
	{
		Level& level = this->levels[i];

		if (this->selectedLevel && *this->selectedLevel == i)
		{
			renderer.setFillColor(255, 255, 100);
		} else
		{
			renderer.setFillColor(100, 100, 100);
		}

		renderer.fillRect(0, 0, level.getWidth(), level.getHeight());

		level.renderStatic(renderer);
		level.renderDynamic(renderer);

		renderer.offset(Vector2(0, level.getHeight() + TILE_MARGIN));
	}
}

void LevelSelector::keyPressed(int key)
{
	if (key == VK_ESCAPE)
	{
		this->close();
	}

	if (key == VK_SPACE)
	{
		this->selectedLevel = new int(0);
	}
}
