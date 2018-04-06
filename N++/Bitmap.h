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

