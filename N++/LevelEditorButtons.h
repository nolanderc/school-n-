#pragma once
#include "MenuButton.h"


enum EditorButtonID
{
	EDITOR_BUTTON_SAVE,
	EDITOR_BUTTON_RUN,
};


class SaveButton: public MenuButton
{
public:

	SaveButton(Vector2 position, double radius);

	void renderImage(Renderer& renderer, Vector2 center, double radius) override;

	int getID() override;
};



class RunButton : public MenuButton
{
public:

	RunButton(Vector2 position, double radius);

	void renderImage(Renderer& renderer, Vector2 center, double radius) override;

	int getID() override;
};
