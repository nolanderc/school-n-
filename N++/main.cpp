
/*
----------------------------------------------
Titel: N++
Av: Christofer Nolander 2018
----------------------------------------------
Story: 
Man �r en ninja som g�r parkour.
----------------------------------------------

Kodens struktur:
Koden �r uppdelad i tre delar: 

- En "backend" som sk�ter allt som har med sj�lva spelet att g�ra (niv�er, kollisioner, navigering av menyer, etc.)
- En "frontend" som sk�ter all interaktion med anv�ndaren (f�nster, grafik, tangentbord, etc.)
- Ett gr�nssnitt som l�ter frontend och backend att kommunicera
 
Dessa delar g�r det m�jligt att endast beh�va koda en backend som fungerar f�r varje platform (Windows, konsol, Mac, lasrar p� m�nen, myror, etc.).
Detta m�jligg�rs genom gr�nssnittet som inte bryr sig om hur en frontend fungerar s� l�nge den kan implementera alla n�dv�ndiga funktioner.

----------------------------------------------

Gr�nssnittet skapar f�ljande klasser som m�ste �rvas av en frontend f�r att den ska kunna interagera med denna backend:
- En f�nsterklass
- En ritarklass

F�nsterklassen �r �mnad f�r en klass som visar allt f�r anv�ndaren och samlar dess "inputs".
Ritarklassen skapas av f�nsterklassen och ger backend-delen m�jligheter att rita till f�nstret.

----------------------------------------------

Gr�nssnittet skapar f�ljande klasser som m�ste �rvas av en backend f�r att den ska kunna ta emot meddelanden ifr�n en frontend:
- En h�ndelsehanterare

H�ndelsehanteraren definierar en m�ngd virtuella funktioner som kan �verlagras f�r att hantera meddelanden ifr�n f�nstret

----------------------------------------------

Gr�nssnittet definierar dessutom en ytterliggare klass:
- App

App-klassen �r t�nkt att ge ett snabbt och enkelt s�tt att dela upp programmet i mindre moduler.
Moduler �r vara sm� delar av ett program som sk�ter en specifik funktion. 
Varje modul �r kopplat direkt till ett f�nster och kan interagera med det direkt.

I detta spel finns f�ljande moduler:
- En huvudmeny
- En tutorial (How to play)
- En niv�redigerare
- En niv�v�ljare
- Ett spel

----------------------------------------------

Varje modul har genom klassen "App" m�jlighet att starta en ny modul innuti sig sj�lv. N�r detta sker
leder klassen "App" alla meddelanden som kommer fr�n f�nstret till den nya modulen samtidigt som den gamla
hamnar i ett "sovande" l�ge. N�r den nya modulen st�ngs "vaknar" den gamla modulen och forts�tter att g�ra
vad den �n gjorde innan.

Denna tr�dliknande struktur g�r det �ven m�jligt f�r en modul att �teranv�ndas av flera andra. Detta sker
bland annat med spelmodulen som anv�nds av b�de niv�v�ljaren och niv�redigeraren.


S� h�r ser hierarkin av alla moduler ut i detta spel:
- Huvudmeny
	- Tutorial
	- Niv�redigerare
		- Spel
	- Niv�v�ljare
		- Spel


Moduler kan snabbt och enkelt bytas ut och l�ggas till allt eftersom det beh�vs.

----------------------------------------------

Hittils har f�ljande frontends implementerats:
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
	// Ta bort oanv�nda parametrar
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrev);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);
	
	// Skapa ett seed till rand
	srand(int(time(nullptr)));

	// Skapa ett f�nster som anv�nder sig av WinAPI.
	// Kunde lika g�rna vara en klass som anv�nder en konsol, ett hologram eller stj�rnorna p� himlen
	WindowsWindow window(512, 512, "N++");
	window.show();

	// Starta huvudmenyn i f�nstret
	MenuApp menu(&window);
	menu.run();
}



