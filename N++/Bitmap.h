#pragma once


// En klass som lagrar en bild
class Bitmap
{
public:

	virtual ~Bitmap() = default;

	virtual int getWidth() = 0;
	virtual int getHeight() = 0;

};
