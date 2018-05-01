#include <ctime>

#include "WindowsWindow.h"
#include "MenuApp.h"

#include <stdio.h>
#include <io.h>
#include <fcntl.h>

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR cmdLine, int nCmdShow)
{
#if 0
	// Trolla fram en konsol
	// Används under utveckling för att hitta buggar :)
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	FILE* file;
	freopen_s(&file, "CON", "w", stdout);
#endif

	// Ta bort oanvända parametrar
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrev);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);
	
	// Skapa ett seed till rand
	srand(time(nullptr));

	// Skapa ett fönster som använder sig av WinAPI.
	// Kunde lika gärna vara en klass som använder en konsol, ett hologram eller stjärnorna på himlen
	WindowsWindow window(512, 512, "N++");
	window.show();

	// Starta huvudmenyn i fönstret
	MenuApp menu(&window);
	menu.run();
}



