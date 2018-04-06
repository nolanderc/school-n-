#include "Window.h"
#include "NinjaGame.h"

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <ctime>

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR cmdLine, int nCmdShow)
{
	if (false) {
		AllocConsole();
		AttachConsole(GetCurrentProcessId());
		FILE* file;
		freopen_s(&file, "CON", "w", stdout);
	}


	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrev);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);
	

	srand(time(nullptr));

	NinjaGame game;
	game.run();
}



// TODO:
// Wall climbing
// Rag doll
// Exit/goal
// Coins
// Rocket launcher
// Seekers

