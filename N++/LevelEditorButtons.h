#pragma once
#include "MenuButton.h"


enum EditorButtonID
{
	EDITOR_BUTTON_SAVE
};


class SaveButton: public MenuButton
{
public:

	SaveButton(Vector2 position, double radius);

	void renderImage(Renderer& renderer, Vector2 center, double radius) override;

	int getID() override;
};
