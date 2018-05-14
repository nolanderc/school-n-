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
	// Niv�n
	Level level;

	// Bitmap att rita niv�n till
	Bitmap* levelBitmap;

	// Att anropa f�r n�r niv�n �r avklarad
	VictoryCallback* victoryCallback;
	
	

	// Lever ninjan?
	bool alive;


	// Lagrar en vinst, om det finns en
	struct Victory {
		double time;
		int coins;

		Victory(double time, int coins) : time(time), coins(coins) {}
	} *victory;

	// Hur l�nge vinsten har visats
	double victoryTime;


	// Spelarens namn
	std::string playerName;



	struct Navigation
	{
		int back;
		int reset;
		int nextLevel;
	} navigation;

	// Navigeringshj�lp
	NavigationBar navBar;


public:

	NinjaGame(App* parent, Level level, VictoryCallback* gameCallback = nullptr, std::string playerName = "");

	~NinjaGame();

	void update(float dt) override;

	void draw(Renderer & renderer) override;

	// Anropas n�r niv�n �r f�rlorad
	void onLevelFail() override;

	// Anropas n�r niv�n �r vunnen
	void onLevelComplete(double time, int coins, std::string name) override;

	// Navigerar
	void navigate(int id) override;

protected:

	void keyPressed(KeyCode key) override;
	void keyReleased(KeyCode key) override;

private:

	void updateFrameCounter(double deltaTime);


	// Ritar vinstsk�rmen
	void renderVictoryScreen(Renderer& renderer);


	// Skapar alla navigeringsknappar
	void createNavBar();

};

