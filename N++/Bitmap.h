#pragma once
#include <Windows.h>
#include <string>

class Bitmap
{
	HBITMAP handle;

	SIZE size;

public:

	Bitmap();

	Bitmap(HBITMAP dc, SIZE size);

	// Ladda en bitmap ifrån hårddisken
	Bitmap(std::string path);

	// Avgör om en bitmap är laddad
	bool isLoaded();

	// Väljer denna bitmap som den nuvarande i en dc
	HBITMAP selectInto(HDC dc);


	// Returnera storleken på denna bild
	SIZE getSize();
	int getWidth();
	int getHeight();
};

