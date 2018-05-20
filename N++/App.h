#pragma once
#include <chrono>

#include "Window.h"


class App: public WindowEventHandler
{
	// F�nstret appen befinner sig i 
	Window* window;

	// Den �nskade storleken p� f�nstret
	Vector2i preferredWindowSize;
	

	// Appen som "�ger" denna app
	App* parent;

	// K�rs appen fortfarande?
	bool running;


	// En app som denna app "�ger"
	App* child;

	// Den kod denna app kommer att avlsluta med
	int exitCode;

public:

	// Skapa en ny app innuti en annan app
	App(App* parent);

	// Skapa en ny app i ett f�nster.
	App(Window* window);

	virtual ~App();

	// K�r appen
	int run();

	// Avslutar appen med en int som resultat kod
	void close(int exitCode = 0);

protected:

	// Uppdatera appen varje 'tick'
	virtual void update(double deltaTime) = 0;

	// Rita om appen
	virtual void draw(Renderer& renderer) = 0;


	// Hantera att appen st�ngdes ned
	virtual void closed() {};

	// Hantera att den underordnade appen st�ngdes ned
	virtual void childClosed(int exitCode) {}

		
	// Anropas n�r delar av f�nstret har f�rsvunnit och m�ste ritas om
	virtual void redraw() {}


	// Returnera f�nstrets storlek
	Vector2i getWindowSize();

	// �ndra f�nstrets storlek
	void setWindowSize(int width, int height);


	// �ndra f�nstrets rubrik/titelrad
	void setWindowTitle(std::string title);
	

	// Avg�r om en knapp �r nedtryckt
	bool isKeyDown(KeyCode key);


	// Returnerar muspekarens position i f�nstret
	Vector2i getMousePosition();



	// Skapar en bitmap som �r kompatibel med f�nstret
	Bitmap* createCompatibleBitmap(Vector2i size);


	// Anger att en ny app ska k�ras innuti denna app.
	void addChild(App* child);


	void alert(std::string title, std::string message);

private:

	// Anropas n�r f�nstret m�ste ritas om
	void repaint() override;

	// Ritar om appen
	void renderApplication();
};

