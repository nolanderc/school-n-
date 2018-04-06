#include "Window.h"
#include "NinjaGame.h"

#include "MenuApp.h"

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <ctime>

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR cmdLine, int nCmdShow)
{
#if 0
		AllocConsole();
		AttachConsole(GetCurrentProcessId());
		FILE* file;
		freopen_s(&file, "CON", "w", stdout);
#endif


	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrev);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);
	

	srand(time(nullptr));

	MenuApp menu;
	menu.run();
}



// TODO:
// Wall climbing
// Rag doll
// Exit/goal
// Coins
// Rocket launcher
// Seekers

