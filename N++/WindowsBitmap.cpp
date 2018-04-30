#include "WindowsBitmap.h"





WindowsBitmap::WindowsBitmap()
{
	this->handle = NULL;
	this->copies = new int(1);
}

WindowsBitmap::~WindowsBitmap()
{
	*this->copies -= 1;
	if (*this->copies == 0)
	{
		delete this->copies;
		DeleteObject(this->handle);
	}
}

WindowsBitmap::WindowsBitmap(const WindowsBitmap& source)
{
	this->handle = source.handle;
	this->size = source.size;
	this->copies = source.copies;

	*this->copies += 1;
}

void WindowsBitmap::operator=(const WindowsBitmap& source)
{
	this->handle = source.handle;
	this->size = source.size;
	this->copies = source.copies;

	*this->copies += 1;
}

WindowsBitmap::WindowsBitmap(HBITMAP bitmap, SIZE size) :
	size(size), handle(bitmap)
{
	this->copies = new int(1);
}


WindowsBitmap::WindowsBitmap(std::string path)
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

bool WindowsBitmap::isLoaded()
{
	return this->handle != NULL;
}

HBITMAP WindowsBitmap::selectInto(HDC dc)
{
	return (HBITMAP)SelectObject(dc, this->handle);
}

SIZE WindowsBitmap::getSize()
{
	return this->size;
}

int WindowsBitmap::getWidth()
{
	return this->size.cx;
}

int WindowsBitmap::getHeight()
{
	return this->size.cy;
}
