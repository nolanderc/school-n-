#pragma once
#include <chrono>

#include "Window.h"


class App: public WindowEventHandler
{
	// F�nstret appen befinner sig i 
	Window* window;
	
public:

	// Skapa en ny app i ett annat f�nster.
	App(Window* window);

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

	// Anger att appen borde ritas om.
	// Anv�nds n�r en app ritas om s�llan
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


private:

	// Anropas n�r f�nstret m�ste ritas om
	void repaint() override;

	// Ritar om appen
	void renderApplication();
};

