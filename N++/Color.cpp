#include "Color.h"
#include "Interpolate.h"

Color::Color() :
	Color(0)
{
}

Color::Color(int grey) :
	Color(grey, grey, grey)
{
}

Color::Color(double grey) :
	Color(grey, grey, grey)
{
}

Color::Color(int r, int g, int b) :
	Color(r / 255.0, g / 255.0, b / 255.0)
{
}

Color::Color(double r, double g, double b) :
	r(r), g(g), b(b)
{
	this->clampColors();
}

void Color::clampColors()
{
	this->r = clamp(this->r, 0.0, 1.0);
	this->g = clamp(this->g, 0.0, 1.0);
	this->b = clamp(this->b, 0.0, 1.0);
}

Color Color::mix(Color color, double amount)
{
	return Color(
		(1.0 - amount) * this->r + amount * color.r,
		(1.0 - amount) * this->g + amount * color.g,
		(1.0 - amount) * this->b + amount * color.b
	);
}
