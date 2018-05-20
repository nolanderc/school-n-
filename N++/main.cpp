
/*
----------------------------------------------
Titel: N++
Av: Christofer Nolander 2018
----------------------------------------------
Story: 
Man är en ninja som gör parkour.
----------------------------------------------

Kodens struktur:
Koden är uppdelad i tre delar: 

- En "backend" som sköter allt som har med själva spelet att göra (nivåer, kollisioner, navigering av menyer, etc.)
- En "frontend" som sköter all interaktion med användaren (fönster, grafik, tangentbord, etc.)
- Ett gränssnitt som låter frontend och backend att kommunicera
 
Dessa delar gör det möjligt att endast behöva koda en backend som fungerar för varje platform (Windows, konsol, Mac, lasrar på månen, myror, etc.).
Detta möjliggörs genom gränssnittet som inte bryr sig om hur en frontend fungerar så länge den kan implementera alla nödvändiga funktioner.

----------------------------------------------

Gränssnittet skapar följande klasser som måste ärvas av en frontend för att den ska kunna interagera med denna backend:
- En fönsterklass
- En ritarklass

Fönsterklassen är ämnad för en klass som visar allt för användaren och samlar dess "inputs".
Ritarklassen skapas av fönsterklassen och ger backend-delen möjligheter att rita till fönstret.

----------------------------------------------

Gränssnittet skapar följande klasser som måste ärvas av en backend för att den ska kunna ta emot meddelanden ifrån en frontend:
- En händelsehanterare

Händelsehanteraren definierar en mängd virtuella funktioner som kan överlagras för att hantera meddelanden ifrån fönstret

----------------------------------------------

Gränssnittet definierar dessutom en ytterliggare klass:
- App

App-klassen är tänkt att ge ett snabbt och enkelt sätt att dela upp programmet i mindre moduler.
Moduler är vara små delar av ett program som sköter en specifik funktion. 
Varje modul är kopplat direkt till ett fönster och kan interagera med det direkt.

I detta spel finns följande moduler:
- En huvudmeny
- En tutorial (How to play)
- En nivåredigerare
- En nivåväljare
- Ett spel

----------------------------------------------

Varje modul har genom klassen "App" möjlighet att starta en ny modul innuti sig själv. När detta sker
leder klassen "App" alla meddelanden som kommer från fönstret till den nya modulen samtidigt som den gamla
hamnar i ett "sovande" läge. När den nya modulen stängs "vaknar" den gamla modulen och fortsätter att göra
vad den än gjorde innan.

Denna trädliknande struktur gör det även möjligt för en modul att återanvändas av flera andra. Detta sker
bland annat med spelmodulen som används av både nivåväljaren och nivåredigeraren.


Så här ser hierarkin av alla moduler ut i detta spel:
- Huvudmeny
	- Tutorial
	- Nivåredigerare
		- Spel
	- Nivåväljare
		- Spel


Moduler kan snabbt och enkelt bytas ut och läggas till allt eftersom det behövs.

----------------------------------------------

Hittils har följande frontends implementerats:
- WinAPI
	- WindowsRenderer
	- WindowsWindow
	- WindowsBitmap

 */




#include <ctime>

#include "WindowsWindow.h"
#include "MenuApp.h"


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR cmdLine, int nCmdShow)
{
	// Ta bort oanvända parametrar
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrev);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);
	
	// Skapa ett seed till rand
	srand(int(time(nullptr)));

	// Skapa ett fönster som använder sig av WinAPI.
	// Kunde lika gärna vara en klass som använder en konsol, ett hologram eller stjärnorna på himlen
	WindowsWindow window(512, 512, "N++");
	window.show();

	// Starta huvudmenyn i fönstret
	MenuApp menu(&window);
	menu.run();
}



