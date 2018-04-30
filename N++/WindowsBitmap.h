#pragma once
#include <Windows.h>
#include <string>
#include "Bitmap.h"

class WindowsBitmap: public Bitmap
{
	// Bitmapsbilden
	HBITMAP handle;

	// Bildens storlek
	SIZE size;


	// Hur m�nga kopior av denna bild som finns
	int* copies;

public:

	WindowsBitmap();
	~WindowsBitmap();

	// En kopia skapas av en bild
	WindowsBitmap(const WindowsBitmap& source);
	void operator=(const WindowsBitmap& source);

	WindowsBitmap(HBITMAP dc, SIZE size);

	// Ladda en bitmap ifr�n h�rddisken
	WindowsBitmap(std::string path);

	// Avg�r om en bitmap �r laddad
	bool isLoaded();

	// V�ljer denna bitmap som den nuvarande i en dc
	HBITMAP selectInto(HDC dc);


	// Returnera storleken p� denna bild
	SIZE getSize();
	int getWidth();
	int getHeight();
};

