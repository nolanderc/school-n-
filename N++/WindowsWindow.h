#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include <vector>

#include "WindowsRenderer.h"
#include "Window.h"


class WindowsWindow: public Window
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
	static std::vector<WindowsWindow*> windows;


	// Lista �ver alla knappar som �r nedtryckta
	std::vector<KeyCode> pressedKeys;


	// Hanterare av f�nstrets meddelanden
	WindowEventHandler* eventHandler;

public:

	WindowsWindow();

	// Skapar ett f�nster
	WindowsWindow(int width, int height, std::string windowName);


	// Visar f�nstret
	void show();
	

	// Hanterar alla meddelanden som har anl�nt sedan sita anropet av denna funktion
	void pollMessages() override;

	// S�tter f�nstrets meddelande hanterare
	void setEventHandler(WindowEventHandler* handler) override;
	

	// Returnerar f�nstrets id nummer
	int getID();

	// Returnerar true om f�nstret �r �ppet
	bool isOpen() override;

	// St�nger f�nstret
	void close() override;

	

	// Returnerar f�nstrets storlek
	Vector2i getSize() override;
	
	// �ndrar f�nstrets storlek
	void setSize(int width, int height) override;


	// �ndrar f�nstrets rubrik/titelrad
	void setTitle(std::string title) override;


	// Returnerar musens nuvarande position
	Vector2i getMousePosition() override;


	// Avg�r om en knapp �r nedtryckt
	bool isKeyDown(KeyCode key) override;


	// Skapar en bitmap kompatibel med f�nstret
	Bitmap* createCompatibleBitmap(Vector2i size) override;


	// Skapar en ny ritare
	Renderer* getNewRenderer() override;

	// Avslutar ritningen
	void submitRenderer(Renderer& renderer) override;

	// Skapar en informationsruta med ett felmeddelande
	void alert(std::string title, std::string message) override;

private:

	// Returnerar f�nstrets "handle"
	HWND getHandle();

	// Windows callback funktion
	static LRESULT CALLBACK windowProcedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	// Skapa en ny window class
	static WNDCLASSEX createClassEx(HINSTANCE hInstance, LPCSTR className);

	// Justerar storleken av f�nstret utifr�n den �nskade storleken p� "f�nsterytan"
	static SIZE adjustSize(int width, int height, DWORD style);

	// Omvandlar en wParam till en tangentkod
	static KeyCode wParamToKeyCode(WPARAM wParam);
};

