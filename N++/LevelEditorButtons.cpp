#include "LevelEditorButtons.h"

SaveButton::SaveButton(Vector2 position, double radius) :
	MenuButton(position, radius, 4)
{
}

void SaveButton::renderImage(Renderer& renderer, Vector2 center, double radius)
{
	double r = radius / 2;

	double left = center.x - r;
	double right = center.x + r;

	double top = center.y - r;
	double bottom = center.y + r;

	renderer.fillPolygon({
		Vector2(left, top + r / 2),
		Vector2(left + r / 2, top),

		Vector2(right, top),
		Vector2(right, bottom),
		Vector2(left, bottom),
	});
}

int SaveButton::getID()
{
	return EDITOR_BUTTON_SAVE;
}

RunButton::RunButton(Vector2 position, double radius) :
	MenuButton(position, radius, 4)
{
}

void RunButton::renderImage(Renderer& renderer, Vector2 center, double radius)
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

int RunButton::getID()
{
	return EDITOR_BUTTON_RUN;
}