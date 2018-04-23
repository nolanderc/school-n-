#pragma once
#include "App.h"
#include "Level.h"
#include "LevelList.h"

#include "NinjaGame.h"
#include "LevelEditor.h"
#include "MenuButtons.h"

#define TILE_SIZE 8
#define TILE_MARGIN 5


class LevelSelector: public App, public VictoryCallback
{
	struct LevelThumbnail
	{
		Level level;

		Bitmap* levelBitmap;

		BoundingBox container;
	};

	// Alla nivåer
	std::vector<LevelThumbnail> levels;

	// Lista över alla spelbara nivåer och deras poäng
	LevelList levelList;

	int* selectedLevel;
	int* highlightedLevel;


	Difficulty difficulty;

	std::vector<BoundingBox> difficultyContainers;


	PlayButton playButton;

public:

	LevelSelector(App* parent);

	~LevelSelector();

	void onLevelComplete(double time, int coins) override;

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
	void createThumbnails(const LevelList& list);

	// Ritar en nivå till en bitmapsbild och returnerar denna
	Bitmap renderLevelThumbnail(Renderer& renderer, Level& level);

	// Ritar alla nivër
	void drawLevels(Renderer& renderer);

	// Omvandlar tid (i sekunder) till en fin string
	std::string formatTime(double time);

	// Ritar ut information om den nivå som är vald
	void drawLevelInformation(Renderer& renderer);

	// Flyttar det valda alternativet
	void changeSelected(int delta);

	// Startar ett spel med en nivå
	void playLevel(int levelIndex);
};
