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


	// Hur många kopior av denna bild som finns
	int* copies;

public:

	WindowsBitmap();
	~WindowsBitmap();

	// En kopia skapas av en bild
	WindowsBitmap(const WindowsBitmap& source);
	void operator=(const WindowsBitmap& source);

	WindowsBitmap(HBITMAP dc, SIZE size);

	// Ladda en bitmap ifrån hårddisken
	WindowsBitmap(std::string path);

	// Avgör om en bitmap är laddad
	bool isLoaded();

	// Väljer denna bitmap som den nuvarande i en dc
	HBITMAP selectInto(HDC dc);


	// Returnera storleken på denna bild
	SIZE getSize();
	int getWidth();
	int getHeight();
};

