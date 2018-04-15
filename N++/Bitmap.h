#pragma once
#include <Windows.h>
#include <string>

class Bitmap
{
	// Bitmapsbilden
	HBITMAP handle;

	// Bildens storlek
	SIZE size;


	// Hur m�nga kopior av denna bild som finns
	int* copies;

public:

	Bitmap();
	~Bitmap();

	// En kopia skapas av en bild
	Bitmap(const Bitmap& source);
	void operator=(const Bitmap& source);

	Bitmap(HBITMAP dc, SIZE size);

	// Ladda en bitmap ifr�n h�rddisken
	Bitmap(std::string path);

	// Avg�r om en bitmap �r laddad
	bool isLoaded();

	// V�ljer denna bitmap som den nuvarande i en dc
	HBITMAP selectInto(HDC dc);


	// Returnera storleken p� denna bild
	SIZE getSize();
	int getWidth();
	int getHeight();
};

