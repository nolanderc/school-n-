#pragma once
#include <Windows.h>
#include <string>

class Bitmap
{
	// Bitmapsbilden
	HBITMAP handle;

	// Bildens storlek
	SIZE size;


	// Hur många kopior av denna bild som finns
	int* copies;

public:

	Bitmap();
	~Bitmap();

	// En kopia skapas av en bild
	Bitmap(const Bitmap& source);
	void operator=(const Bitmap& source);

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

