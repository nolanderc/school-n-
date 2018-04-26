#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include <vector>

#include "Renderer.h"

enum MouseButton {
	MOUSE_LEFT,
	MOUSE_MIDDLE,
	MOUSE_RIGHT
};


// Hanterar meddelanden som kommer ifr�n ett f�nster
class WindowEventHandler {
public:
	virtual ~WindowEventHandler() = default;

	// F�nstret m�ste ritas om
	virtual void repaint() = 0;


	// F�nstret �ndrade storlek
	virtual void sizeChanged(int width, int height) {}


	// En tangentbordsknapp trycktes
	virtual void keyPressed(int key) {}

	// En tangentbordsknapp sl�pptes
	virtual void keyReleased(int key) {}

	// En musknapp trycktes eller sl�pptes
	virtual void mousePressed(MouseButton button, int x, int y) {}
	virtual void mouseReleased(MouseButton button, int x, int y) {}

	// Muspekaren r�rde sig
	virtual void mouseMoved(int x, int y) {}

	// Musens hjul r�rde sig
	virtual void mouseScrolled(int wheelDelta, int x, int y) {}
};


class Window
{
	// F�nstrets "handle"
	HWND handle;

	// F�nstrets nuvarande dc
	HDC dc;

	// F�nstrets backbuffer, en bitmapsbild som allt ritas till 
	HBITMAP backBuffer;

	// F�nstrets id nummer
	int id;

	// F�nstrets "stil"
	DWORD style;

	// Avg�r om f�nstret �r �ppet eller inte
	bool open;

	// Storleken av f�nstret
	SIZE size;


	// Musens position i f�nstret
	Vector2i mousePosition;


	// Lista �ver alla f�nster
	static std::vector<Window*> windows;


	// Lista �ver alla knappar som �r nedtryckta
	std::vector<int> pressedKeys;


	// Hanterare av f�nstrets meddelanden
	WindowEventHandler* eventHandler;

public:

	Window();

	// Skapar ett f�nster
	Window(int width, int height, std::string windowName);


	// Visar f�nstret
	void show(int nCmdShow);
	

	// V�ntar p� att meddelande ska anl�nda och hanterar sedan dessa
	void pollMessages();

	// S�tter f�nstrets meddelande hanterare
	void setEventHandler(WindowEventHandler* handler);
	

	// Returnerar f�nstrets id nummer
	int getID();

	// Returnerar true om f�nstret �r �ppet
	bool isOpen();

	// St�nger f�nstret
	void close();

	

	// Returnerar f�nstrets storlek
	SIZE getSize();
	
	// �ndrar f�nstrets storlek
	void setSize(int width, int height);


	// �ndrar f�nstrets rubrik/titelrad
	void setTitle(std::string title);


	// Returnerar musens nuvarande position
	POINT getMousePosition();


	// Avg�r om en knapp �r nedtryckt
	bool isKeyDown(int key);


	// Skapar en bitmap kompatibel med f�nstret
	Bitmap createCompatibleBitmap(SIZE size);


	// Skapar en ny ritare
	Renderer getNewRenderer();

	// Avslutar ritningen
	void submitRenderer(Renderer& renderer);

protected:

	// Returnerar f�nstrets "handle"
	HWND getHandle();

private:

	// Windows callback funktion
	static LRESULT CALLBACK windowProcedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	// Skapa en ny window class
	static WNDCLASSEX createClassEx(HINSTANCE hInstance, LPCSTR className);

	// Justerar storleken av f�nstret utifr�n den �nskade storleken p� "f�nsterytan"
	static SIZE adjustSize(int width, int height, DWORD style);

};

