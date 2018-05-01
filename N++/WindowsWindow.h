#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include <vector>

#include "WindowsRenderer.h"
#include "Window.h"


class WindowsWindow: public Window
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
	static std::vector<WindowsWindow*> windows;


	// Lista över alla knappar som är nedtryckta
	std::vector<KeyCode> pressedKeys;


	// Hanterare av fönstrets meddelanden
	WindowEventHandler* eventHandler;

public:

	WindowsWindow();

	// Skapar ett fönster
	WindowsWindow(int width, int height, std::string windowName);


	// Visar fönstret
	void show();
	

	// Hanterar alla meddelanden som har anlänt sedan sita anropet av denna funktion
	void pollMessages() override;

	// Sätter fönstrets meddelande hanterare
	void setEventHandler(WindowEventHandler* handler) override;
	

	// Returnerar fönstrets id nummer
	int getID();

	// Returnerar true om fönstret är öppet
	bool isOpen() override;

	// Stänger fönstret
	void close() override;

	

	// Returnerar fönstrets storlek
	Vector2i getSize() override;
	
	// Ändrar fönstrets storlek
	void setSize(int width, int height) override;


	// Ändrar fönstrets rubrik/titelrad
	void setTitle(std::string title) override;


	// Returnerar musens nuvarande position
	Vector2i getMousePosition() override;


	// Avgör om en knapp är nedtryckt
	bool isKeyDown(KeyCode key) override;


	// Skapar en bitmap kompatibel med fönstret
	Bitmap* createCompatibleBitmap(Vector2i size) override;


	// Skapar en ny ritare
	Renderer* getNewRenderer() override;

	// Avslutar ritningen
	void submitRenderer(Renderer& renderer) override;

	// Skapar en informationsruta med ett felmeddelande
	void alert(std::string title, std::string message) override;

private:

	// Returnerar fönstrets "handle"
	HWND getHandle();

	// Windows callback funktion
	static LRESULT CALLBACK windowProcedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	// Skapa en ny window class
	static WNDCLASSEX createClassEx(HINSTANCE hInstance, LPCSTR className);

	// Justerar storleken av fönstret utifrån den önskade storleken på "fönsterytan"
	static SIZE adjustSize(int width, int height, DWORD style);

	// Omvandlar en wParam till en tangentkod
	static KeyCode wParamToKeyCode(WPARAM wParam);
};

