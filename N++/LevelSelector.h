#pragma once
#include "App.h"
#include "Level.h"
#include "LevelList.h"

#include "NinjaGame.h"
#include "LevelEditor.h"
#include "MenuButtons.h"

#define TILE_SIZE 8
#define TILE_MARGIN 5

#define STAR_SIZE (2 * TILE_MARGIN)


class LevelSelector : public App, public VictoryCallback
{
	struct LevelThumbnail
	{
		Level level;

		Bitmap* levelBitmap;

		BoundingBox container;
		BoundingBox starContainer;

		int starCount;

		~LevelThumbnail();
	};

	// Alla nivåer
	std::vector<LevelThumbnail> levels;

	// Antalet avklarade nivåer
	int completedLevelCount;

	// Lista över alla spelbara nivåer och deras poäng
	LevelList levelList;

	// Den markerade nivån
	int* selectedLevel;

	// Den nivå som är under muspekaren
	int* highlightedLevel;


	// Hur långt i listan som har skrollats
	double scrollAmount;

	// Hur långt i listan som borde skrollats
	double scrollTarget;

	// Hur långt ner som man kan skrolla
	double maxScroll;



	struct Highscore {
		int level;
		Difficulty difficulty;
		int score;

		Highscore(int level, Difficulty difficulty, int score) :
			level(level), difficulty(difficulty), score(score) {}
	} *recentHighscore;


	// Den valda svårighetsgraden
	Difficulty difficulty;


	// Rektanglar där man kan klicka för att välja svårighetsgrad
	std::vector<BoundingBox> difficultyContainers;


	// Spela-knappen
	PlayButton playButton;


	// Text som visas under muspekaren
	std::string tooltip;

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

	void mouseScrolled(int wheelDelta, int x, int y);

	void keyPressed(int key) override;


private:

	// Skrollar i listan av nivåer
	void scroll(double delta);

	// Anger att listan borde skrollas visst mycket
	void addScroll(double delta);

	// Ändrar den nuvarande svårighetsgraden
	void changeDifficulty(Difficulty difficulty);


	// Avgör hur många stjärnor en nivå har
	int getLevelStarCount(int level);

	// Avgör hur många nivåer som är avklarade
	void checkCompletedLevels();


	// Skapar allt som tillhör informationspanelen
	void createInformationPane();


	// Skapar alla bilder och kollisionslådor för nivåerna
	void createThumbnails(const LevelList& list);

	// Ritar en nivå till en bitmapsbild och returnerar denna
	Bitmap* renderLevelThumbnail(Renderer& renderer, Level& level);

	// Ritar ett lås innuti en rektangel
	void drawLock(Renderer& renderer, const BoundingBox& container, Color backgroundColor);


	// Ritar en stjärna
	void fillStar(Renderer& renderer, Vector2 center, double radius);


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
