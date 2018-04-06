#include "MenuButton.h"

MenuButton::MenuButton(Vector2 position, double radius)
{
	std::vector<Vector2> points;
	int pointCount = 8;
	for (int i = 0; i < pointCount; i++)
	{
		double angle = i / double(pointCount) * 360 + 22.5;

		points.push_back(radius * Vector2::rotated(angle) + position);
	}

	this->container = ConvexHull(points);

	this->color = { 20, 20, 200 };
	this->highlightColor = { 100, 100, 255 };
}

void MenuButton::update(double deltaTime)
{
	double delta = this->highlighted ? deltaTime : -deltaTime;
	this->highlightAmount = clamp(this->highlightAmount + 6 * delta, 0, 1);
}

void MenuButton::render(Renderer& renderer)
{
	Color result;
	result.r = easeInOut(this->highlightAmount, this->color.r, this->highlightColor.r);
	result.g = easeInOut(this->highlightAmount, this->color.g, this->highlightColor.g);
	result.b = easeInOut(this->highlightAmount, this->color.b, this->highlightColor.b);

	double extraSize = easeInOut(this->highlightAmount, 0, 10);

	BoundingBox bounds = this->container.getBoundingBox();
	double radius = (bounds.right - bounds.left) / 2 + extraSize;

	Vector2 position = this->container.average();

	std::vector<Vector2> points;
	int pointCount = 8;
	for (int i = 0; i < pointCount; i++)
	{
		double angle = i / double(pointCount) * 360 + 22.5;

		points.push_back(radius * Vector2::rotated(angle) + position);
	}

	renderer.setFillColor(result.r, result.g, result.b);
	renderer.fillPolygon(points);


	renderer.setFillColor(255, 255, 255);
	this->renderImage(renderer, position, radius);
}

bool MenuButton::contains(Vector2 point)
{
	return this->container.contains(point);
}

void MenuButton::setHighlight(bool highlight)
{
	this->highlighted = highlight;
}
