#pragma once
#include <chrono>

#include "Window.h"


class App: public WindowEventHandler
{
	// F�nstret appen befinner sig i 
	Window* window;

	// Den �nskade storleken p� f�nstret
	SIZE preferredWindowSize;


	// Appen som "�ger" denna app
	App* parent;

	// K�rs appen fortfarande?
	bool running;


	// En app som denna app "�ger"
	App* child;

public:

	// Skapa en ny app innuti en annan app
	App(App* parent);

	// Skapa en ny app i ett nytt f�nster.
	App(int width, int height, std::string title);

	
	// K�r appen
	void run();

	// Avslutar appen
	void close();

protected:

	// Uppdatera appen varje 'tick'
	virtual void update(float deltaTime) = 0;

	// Rita om appen
	virtual void draw(Renderer& renderer) = 0;


	// Hantera f�nstrets nya storlek
	void sizeChanged(int width, int height) override {}


	// Hantera knapptryck
	void keyPressed(int key) override {}

	// Hantera musknappar
	void mousePressed(MouseButton button, int x, int y) override {}

	// Hantera att muspekaren r�rde sig
	void mouseMoved(int x, int y) override {}

	
	// Anropas n�r delar av f�nstret har f�rsvunnit och m�ste ritas om
	virtual void redraw() {}


	// Returnera f�nstrets storlek
	SIZE getWindowSize();

	// �ndra f�nstrets storlek
	void setWindowSize(int width, int height);


	// �ndra f�nstrets rubrik/titelrad
	void setWindowTitle(std::string title);
	

	// Avg�r om en knapp �r nedtryckt
	bool isKeyDown(int key);


	// Skapar en bitmap som �r kompatibel med f�nstret
	Bitmap createCompatibleBitmap(SIZE size);


	// Anger att en ny app ska k�ras innuti denna app
	void addChild(App* child);

private:

	// Anropas n�r f�nstret m�ste ritas om
	void repaint() override;

	// Ritar om appen
	void renderApplication();
};

