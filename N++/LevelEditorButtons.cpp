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
