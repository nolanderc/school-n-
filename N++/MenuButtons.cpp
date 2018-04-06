#include "MenuButtons.h"

PlayButton::PlayButton(Vector2 position, double radius) :
	MenuButton(position, radius)
{
}

void PlayButton::renderImage(Renderer& renderer, Vector2 center, double radius)
{
	double x = center.x;
	double y = center.y;

	double r = radius;

	double left = x - r / 4.5;
	
	renderer.fillPolygon({
		Vector2(left, y - r / 2),
		Vector2(left + r / 2, y),
		Vector2(left, y + r / 2),
	});
}

int PlayButton::getID()
{
	return BUTTON_PLAY;
}


ExitButton::ExitButton(Vector2 position, double radius) :
	MenuButton(position, radius)
{
}

void ExitButton::renderImage(Renderer& renderer, Vector2 center, double radius)
{
	double x = center.x;
	double y = center.y;

	double r = radius / 3;

	double left = x - r;
	double right = x + r;
	double top = y - r;
	double bottom = y + r;

	renderer.setColor(255, 0, 0);
	renderer.setLineStyle(LINE_SOLID);
	renderer.setLineWidth(10);

	renderer.drawLine(left, top, right, bottom);
	renderer.drawLine(left, bottom, right, top);
}

int ExitButton::getID()
{
	return BUTTON_EXIT;
}