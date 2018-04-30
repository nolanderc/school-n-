#pragma once
#include "App.h"

#include "ConvexHull.h"
#include "Level.h"

#include "Ninja.h"

#define TILE_SIZE 32

#define ENERGY_BAR_HEIGHT_PIXELS 16




class NinjaGame: public App, public VictoryCallback
{
	// Niv�n
	Level level;

	// Bitmap att rita niv�n till
	Bitmap* levelBitmap;

	// Borde niv�n ritas om?
	bool renderLevel;

	// Att anropa f�r n�r niv�n �r avklarad
	VictoryCallback* gameCallback;
	

	// Lagrar en vinst, om det finns en
	struct Victory {
		double time;
		int coins;

		Victory(double time, int coins) : time(time), coins(coins) {}
	} *victory;

public:

	NinjaGame(App* parent, Level level, VictoryCallback* gameCallback = nullptr);

	~NinjaGame();

	void update(float dt) override;

	void draw(Renderer & renderer) override;



	// Anropas n�r niv�n �r vunnen
	virtual void onLevelComplete(double time, int coins) override;

protected:

	void sizeChanged(int width, int height) override;

	void keyPressed(int key) override;
	void keyReleased(int key) override;

private:

	void updateFrameCounter(double deltaTime);


	// Ritar vinstsk�rmen
	void renderVictoryScreen(Renderer& renderer);

};

