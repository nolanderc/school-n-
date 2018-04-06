#pragma once
#include <chrono>

#include "Window.h"


class App: public WindowEventHandler
{
	// Fönstret appen befinner sig i 
	Window* window;
	
public:

	// Skapa en ny app i ett annat fönster.
	App(Window* window);

	// Skapa en ny app i ett nytt fönster.
	App(int width, int height, std::string title);

	
	// Kör appen
	void run();

	// Avslutar appen
	void close();

protected:

	// Uppdatera appen varje 'tick'
	virtual void update(float deltaTime) = 0;

	// Rita om appen
	virtual void draw(Renderer& renderer) = 0;


	// Hantera fönstrets nya storlek
	void sizeChanged(int width, int height) override {}


	// Hantera knapptryck
	void keyPressed(int key) override {}

	// Hantera musknappar
	void mousePressed(MouseButton button, int x, int y) override {}

	// Hantera att muspekaren rörde sig
	void mouseMoved(int x, int y) override {}

	// Anger att appen borde ritas om.
	// Används när en app ritas om sällan
	virtual void redraw() {}


	// Returnera fönstrets storlek
	SIZE getWindowSize();

	// Ändra fönstrets storlek
	void setWindowSize(int width, int height);


	// Ändra fönstrets rubrik/titelrad
	void setWindowTitle(std::string title);


	// Avgör om en knapp är nedtryckt
	bool isKeyDown(int key);


	// Skapar en bitmap som är kompatibel med fönstret
	Bitmap createCompatibleBitmap(SIZE size);


private:

	// Anropas när fönstret måste ritas om
	void repaint() override;

	// Ritar om appen
	void renderApplication();
};

