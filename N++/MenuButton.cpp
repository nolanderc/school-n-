#include "MenuButton.h"

MenuButton::MenuButton(Vector2 position, double radius) :
	center(position), radius(radius), highlighted(false), highlightAmount(0), selected(false), selectionAmount(0)
{
	this->container = createHull(this->radius);

	this->color = { 20, 20, 200 };
	this->highlightColor = { 100, 100, 255 };
}

void MenuButton::update(double deltaTime)
{
	double delta = this->highlighted ? deltaTime : -deltaTime;
	this->highlightAmount = clamp(this->highlightAmount + 6 * delta, 0, 1);

	delta = this->selected ? deltaTime : -deltaTime;
	this->selectionAmount = clamp(this->selectionAmount + 10 * delta, 0, 1);

	double extraSize = easeInOut(this->highlightAmount, 0, -10) + easeInOut(this->selectionAmount, 0, -10);
	this->actualRadius = this->radius + extraSize;
}

void MenuButton::render(Renderer& renderer)
{
	Color result;
	result.r = easeInOut(this->highlightAmount, this->color.r, this->highlightColor.r);
	result.g = easeInOut(this->highlightAmount, this->color.g, this->highlightColor.g);
	result.b = easeInOut(this->highlightAmount, this->color.b, this->highlightColor.b);

	renderer.setFillColor(result.r, result.g, result.b);
	createHull(this->actualRadius).fill(renderer);
	
	renderer.setFillColor(255, 255, 255);
	this->renderImage(renderer, this->center, this->actualRadius);
}

bool MenuButton::contains(Vector2 point)
{
	return this->container.contains(point);
}

void MenuButton::setHighlight(bool highlight)
{
	this->highlighted = highlight;
}

void MenuButton::setSelected(bool selected)
{
	this->selected = selected;
}

ConvexHull MenuButton::createHull(double radius)
{
	std::vector<Vector2> points;
	int pointCount = 6;
	for (int i = 0; i < pointCount; i++)
	{
		double angle = i / double(pointCount) * 360 + 180 / double(pointCount);

		points.push_back(radius * Vector2::rotated(angle) + this->center);
	}

	return ConvexHull(points);
}
