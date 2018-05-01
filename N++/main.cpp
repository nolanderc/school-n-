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
	// Anv�nds under utveckling f�r att hitta buggar :)
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	FILE* file;
	freopen_s(&file, "CON", "w", stdout);
#endif

	// Ta bort oanv�nda parametrar
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrev);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);
	
	// Skapa ett seed till rand
	srand(time(nullptr));

	// Skapa ett f�nster som anv�nder sig av WinAPI.
	// Kunde lika g�rna vara en klass som anv�nder en konsol, ett hologram eller stj�rnorna p� himlen
	WindowsWindow window(512, 512, "N++");
	window.show();

	// Starta huvudmenyn i f�nstret
	MenuApp menu(&window);
	menu.run();
}



