#include "BoundingBox.h"


BoundingBox::BoundingBox() :
	left(INFINITY), right(-INFINITY), top(INFINITY), bottom(-INFINITY)
{
}

BoundingBox::BoundingBox(double left, double right, double top, double bottom) :
	left(left), right(right), top(top), bottom(bottom)
{
}

bool BoundingBox::intersects(const BoundingBox & other) const
{
	return this->left <= other.right && other.left <= this->right &&
		this->top <= other.bottom && other.top <= this->bottom;
}

void BoundingBox::translate(Vector2 amount)
{
	this->left += amount.x;
	this->right+= amount.x;
	this->top += amount.y;
	this->bottom += amount.y;
}

bool BoundingBox::contains(Vector2 point)
{
	return this->left <= point.x && point.x <= this->right &&
		this->top <= point.y && point.y <= this->bottom;
}
