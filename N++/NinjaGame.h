#pragma once
#include "App.h"

#include "ConvexHull.h"
#include "Level.h"

#include "Ninja.h"
#include "NavigationBar.h"

#define TILE_SIZE 32

#define ENERGY_BAR_HEIGHT_PIXELS 16



enum NinjaGameExitCode {
	NINJA_GAME_NOTHING,
	NINJA_GAME_NEXT_LEVEL
};


class NinjaGame: public App, public VictoryCallback, public NavigationCallback
{
	// Nivån
	Level level;

	// Bitmap att rita nivån till
	Bitmap* levelBitmap;

	// Att anropa för när nivån är avklarad
	VictoryCallback* victoryCallback;
	
	

	// Lever ninjan?
	bool alive;


	// Lagrar en vinst, om det finns en
	struct Victory {
		double time;
		int coins;

		Victory(double time, int coins) : time(time), coins(coins) {}
	} *victory;

	// Hur länge vinsten har visats
	double victoryTime;


	// Spelarens namn
	std::string playerName;



	struct Navigation
	{
		int back;
		int reset;
		int nextLevel;
	} navigation;

	// Navigeringshjälp
	NavigationBar navBar;


public:

	NinjaGame(App* parent, Level level, VictoryCallback* gameCallback = nullptr, std::string playerName = "");

	~NinjaGame();

	void update(float dt) override;

	void draw(Renderer & renderer) override;

	// Anropas när nivån är förlorad
	void onLevelFail() override;

	// Anropas när nivån är vunnen
	void onLevelComplete(double time, int coins, std::string name) override;

	// Navigerar
	void navigate(int id) override;

protected:

	void keyPressed(KeyCode key) override;
	void keyReleased(KeyCode key) override;

private:

	void updateFrameCounter(double deltaTime);


	// Ritar vinstskärmen
	void renderVictoryScreen(Renderer& renderer);


	// Skapar alla navigeringsknappar
	void createNavBar();

};

