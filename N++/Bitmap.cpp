#include "Bitmap.h"





Bitmap::Bitmap()
{
	this->handle = NULL;
}

Bitmap::Bitmap(HBITMAP bitmap, SIZE size) :
	size(size), handle(bitmap)
{
}


Bitmap::Bitmap(std::string path)
{
	HBITMAP img = (HBITMAP)LoadImage(NULL, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (img) {
		this->handle = img;
	}
	else {
		MessageBox(NULL, "Failed to load image", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
}

bool Bitmap::isLoaded()
{
	return this->handle != NULL;
}

HBITMAP Bitmap::selectInto(HDC dc)
{
	return (HBITMAP)SelectObject(dc, this->handle);
}

SIZE Bitmap::getSize()
{
	return this->size;
}

int Bitmap::getWidth()
{
	return this->size.cx;
}

int Bitmap::getHeight()
{
	return this->size.cy;
}
