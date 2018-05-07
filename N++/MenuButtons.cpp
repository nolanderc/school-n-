#include "MenuButtons.h"

PlayButton::PlayButton(Vector2 position, double radius) :
	MenuButton(position, radius, 6)
{
}

void PlayButton::renderImage(Renderer& renderer, Vector2 center, double radius)
{
	double x = center.x;
	double y = center.y;

	double r = radius;

	double left = x - r / 4.5;
	
	renderer.setFillColor(0, 255, 0);
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


/////////////////////////////////////////


HelpButton::HelpButton(Vector2 position, double radius) :
	MenuButton(position, radius, 6)
{
}

void HelpButton::renderImage(Renderer& renderer, Vector2 center, double radius)
{
	double x = center.x;
	double y = center.y;

	double r = radius / 1.5;

	double s = r / 5;
	
	renderer.setFillColor(255, 255, 0);

	// Rita ett frågetecken
	renderer.fillPolygon({
		Vector2(x - 1.5 * s, y - 1 * s),
		Vector2(x - 1.5 * s, y - 2 * s),
		Vector2(x - 1 * s, y - 2.5 * s),

		Vector2(x + 1 * s, y - 2.5 * s),
		Vector2(x + 1.5 * s, y - 2 * s),
		Vector2(x + 1.5 * s, y - 0 * s),

		Vector2(x + 0.5 * s, y + 1 * s),
		Vector2(x + 0.5 * s, y + 1.5 * s),
		Vector2(x - 0.5 * s, y + 1.5 * s),

		Vector2(x - 0.5 * s, y + 0.5 * s),
		Vector2(x + 0.5 * s, y - 0.5 * s),

		Vector2(x + 0.5 * s, y - 1.5 * s),
		Vector2(x - 0.5 * s, y - 1.5 * s),

		Vector2(x - 0.5 * s, y - 1 * s),
	});

	renderer.fillPolygon({
		Vector2(x - 0.5 * s, y + 2.5 * s),
		Vector2(x + 0.5 * s, y + 2.5 * s),
		Vector2(x + 0.5 * s, y + 3.5 * s),
		Vector2(x - 0.5 * s, y + 3.5 * s),
	});
}

int HelpButton::getID()
{
	return BUTTON_HELP;
}


/////////////////////////////////////////



EditorButton::EditorButton(Vector2 position, double radius) :
	MenuButton(position, radius, 6)
{
}


void EditorButton::renderImage(Renderer& renderer, Vector2 center, double radius)
{

	double x = center.x;
	double y = center.y;

	double r = radius / 3;

	double left = x - r;
	double right = x + r;
	double top = y - r;
	double bottom = y + r;

	double margin = 2;
	double size = (right - left - 2 * margin) / 3;

	double paddingTop = ((bottom - top) - (2 * size + 1 * margin)) / 2;

	x = left;
	y = top + paddingTop;

	renderer.setFillColor(255, 0, 255);

	renderer.fillRect(x + size + margin, y, size, size);
	renderer.fillRect(x + 2 * size + 2 * margin, y, size, size);

	renderer.fillRect(x, y + size + margin, size, size);
	renderer.fillRect(x + size + margin, y + size + margin, size, size);
	renderer.fillRect(x + 2 * size + 2 * margin, y + size + margin, size, size);
}

int EditorButton::getID()
{
	return BUTTON_EDITOR;
}


/////////////////////////////////////////


ExitButton::ExitButton(Vector2 position, double radius) :
	MenuButton(position, radius, 6)
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