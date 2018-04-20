﻿#pragma once
#include "App.h"
#include "Level.h"

#include "NinjaGame.h"
#include "LevelEditor.h"
#include "MenuButtons.h"

#define TILE_SIZE 8
#define TILE_MARGIN 5


class LevelSelector: public App
{
	struct LevelThumbnail
	{
		Level level;

		Bitmap* levelBitmap;

		BoundingBox container;
	};

	std::vector<LevelThumbnail> levels;

	int* selectedLevel;
	int* highlightedLevel;


	Difficulty difficulty;

	std::vector<BoundingBox> difficultyContainers;


	PlayButton playButton;

public:

	LevelSelector(App* parent);

	~LevelSelector();

protected:

	void update(float deltaTime) override;

	void draw(Renderer& renderer) override;


	void mouseMoved(int x, int y) override;

	void mousePressed(MouseButton button, int x, int y) override;
	void mouseReleased(MouseButton button, int x, int y) override;

	void keyPressed(int key) override;


private:

	// Skapar allt som tillhör informationspanelen
	void createInformationPane();


	// Skapar alla bilder och kollisionslådor för nivåerna
	void createThumbnails(const std::vector<std::string>& paths);

	// Ritar en nivå till en bitmapsbild och returnerar denna
	Bitmap renderLevelThumbnail(Renderer& renderer, Level& level);

	// Ritar alla nivër
	void drawLevels(Renderer& renderer);

	// Ritar ut information om den nivå som är vald
	void drawLevelInformation(Renderer& renderer);

	// Flyttar det valda alternativet
	void changeSelected(int delta);

	// Startar ett spel med en nivå
	void playLevel(int levelIndex);
};
