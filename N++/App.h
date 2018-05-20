#pragma once
#include <chrono>

#include "Window.h"


class App: public WindowEventHandler
{
	// Fönstret appen befinner sig i 
	Window* window;

	// Den önskade storleken på fönstret
	Vector2i preferredWindowSize;
	

	// Appen som "äger" denna app
	App* parent;

	// Körs appen fortfarande?
	bool running;


	// En app som denna app "äger"
	App* child;

	// Den kod denna app kommer att avlsluta med
	int exitCode;

public:

	// Skapa en ny app innuti en annan app
	App(App* parent);

	// Skapa en ny app i ett fönster.
	App(Window* window);

	virtual ~App();

	// Kör appen
	int run();

	// Avslutar appen med en int som resultat kod
	void close(int exitCode = 0);

protected:

	// Uppdatera appen varje 'tick'
	virtual void update(double deltaTime) = 0;

	// Rita om appen
	virtual void draw(Renderer& renderer) = 0;


	// Hantera att appen stängdes ned
	virtual void closed() {};

	// Hantera att den underordnade appen stängdes ned
	virtual void childClosed(int exitCode) {}

		
	// Anropas när delar av fönstret har försvunnit och måste ritas om
	virtual void redraw() {}


	// Returnera fönstrets storlek
	Vector2i getWindowSize();

	// Ändra fönstrets storlek
	void setWindowSize(int width, int height);


	// Ändra fönstrets rubrik/titelrad
	void setWindowTitle(std::string title);
	

	// Avgör om en knapp är nedtryckt
	bool isKeyDown(KeyCode key);


	// Returnerar muspekarens position i fönstret
	Vector2i getMousePosition();



	// Skapar en bitmap som är kompatibel med fönstret
	Bitmap* createCompatibleBitmap(Vector2i size);


	// Anger att en ny app ska köras innuti denna app.
	void addChild(App* child);


	void alert(std::string title, std::string message);

private:

	// Anropas när fönstret måste ritas om
	void repaint() override;

	// Ritar om appen
	void renderApplication();
};

