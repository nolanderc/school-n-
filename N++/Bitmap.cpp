#include "Bitmap.h"





Bitmap::Bitmap()
{
	this->handle = NULL;
	this->copies = new int(1);
}

Bitmap::~Bitmap()
{
	*this->copies -= 1;
	if (*this->copies == 0)
	{
		delete this->copies;
		DeleteObject(this->handle);
	}
}

Bitmap::Bitmap(Bitmap& source)
{
	this->handle = source.handle;
	this->size = source.size;
	this->copies = source.copies;

	*this->copies += 1;
}

void Bitmap::operator=(Bitmap& source)
{
	this->handle = source.handle;
	this->size = source.size;
	this->copies = source.copies;

	*this->copies += 1;
}

Bitmap::Bitmap(HBITMAP bitmap, SIZE size) :
	size(size), handle(bitmap)
{
	this->copies = new int(1);
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
