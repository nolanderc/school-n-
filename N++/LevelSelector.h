#pragma once
#include "App.h"
#include "Level.h"

#include "NinjaGame.h"

#define TILE_SIZE 8
#define TILE_MARGIN 5


class LevelSelector: public App
{
	struct LevelThumbnail
	{
		Level level;
		BoundingBox container;
		BoundingBox playContainer;
	};


	std::vector<LevelThumbnail> levels;

	int* selectedLevel;

public:
	
	LevelSelector(App* parent);

	~LevelSelector();

protected:

	void update(float deltaTime) override;

	void draw(Renderer& renderer) override;


	void mouseMoved(int x, int y) override;
	void mousePressed(MouseButton button, int x, int y) override;

	void keyPressed(int key) override;


private:

	// Skapar alla bilder och kollisionslådor för nivåerna
	void createThumbnails(const std::vector<std::string>& paths);
};
