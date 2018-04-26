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


// Hanterar meddelanden som kommer ifrån ett fönster
class WindowEventHandler {
public:
	virtual ~WindowEventHandler() = default;

	// Fönstret måste ritas om
	virtual void repaint() = 0;


	// Fönstret ändrade storlek
	virtual void sizeChanged(int width, int height) {}


	// En tangentbordsknapp trycktes
	virtual void keyPressed(int key) {}

	// En tangentbordsknapp släpptes
	virtual void keyReleased(int key) {}

	// En musknapp trycktes eller släpptes
	virtual void mousePressed(MouseButton button, int x, int y) {}
	virtual void mouseReleased(MouseButton button, int x, int y) {}

	// Muspekaren rörde sig
	virtual void mouseMoved(int x, int y) {}

	// Musens hjul rörde sig
	virtual void mouseScrolled(int wheelDelta, int x, int y) {}
};


class Window
{
	// Fönstrets "handle"
	HWND handle;

	// Fönstrets nuvarande dc
	HDC dc;

	// Fönstrets backbuffer, en bitmapsbild som allt ritas till 
	HBITMAP backBuffer;

	// Fönstrets id nummer
	int id;

	// Fönstrets "stil"
	DWORD style;

	// Avgör om fönstret är öppet eller inte
	bool open;

	// Storleken av fönstret
	SIZE size;


	// Musens position i fönstret
	Vector2i mousePosition;


	// Lista över alla fönster
	static std::vector<Window*> windows;


	// Lista över alla knappar som är nedtryckta
	std::vector<int> pressedKeys;


	// Hanterare av fönstrets meddelanden
	WindowEventHandler* eventHandler;

public:

	Window();

	// Skapar ett fönster
	Window(int width, int height, std::string windowName);


	// Visar fönstret
	void show(int nCmdShow);
	

	// Väntar på att meddelande ska anlända och hanterar sedan dessa
	void pollMessages();

	// Sätter fönstrets meddelande hanterare
	void setEventHandler(WindowEventHandler* handler);
	

	// Returnerar fönstrets id nummer
	int getID();

	// Returnerar true om fönstret är öppet
	bool isOpen();

	// Stänger fönstret
	void close();

	

	// Returnerar fönstrets storlek
	SIZE getSize();
	
	// Ändrar fönstrets storlek
	void setSize(int width, int height);


	// Ändrar fönstrets rubrik/titelrad
	void setTitle(std::string title);


	// Returnerar musens nuvarande position
	POINT getMousePosition();


	// Avgör om en knapp är nedtryckt
	bool isKeyDown(int key);


	// Skapar en bitmap kompatibel med fönstret
	Bitmap createCompatibleBitmap(SIZE size);


	// Skapar en ny ritare
	Renderer getNewRenderer();

	// Avslutar ritningen
	void submitRenderer(Renderer& renderer);

protected:

	// Returnerar fönstrets "handle"
	HWND getHandle();

private:

	// Windows callback funktion
	static LRESULT CALLBACK windowProcedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	// Skapa en ny window class
	static WNDCLASSEX createClassEx(HINSTANCE hInstance, LPCSTR className);

	// Justerar storleken av fönstret utifrån den önskade storleken på "fönsterytan"
	static SIZE adjustSize(int width, int height, DWORD style);

};

