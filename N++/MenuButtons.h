#pragma once
#include "MenuButton.h"

enum MenuButtonType
{
	BUTTON_PLAY,
	BUTTON_HELP,
	BUTTON_EXIT,
	BUTTON_EDITOR
};


class PlayButton: public MenuButton
{
public:

	PlayButton(Vector2 position, double radius);

	void renderImage(Renderer& renderer, Vector2 center, double radius) override;

	int getID() override;
};


class HelpButton : public MenuButton
{
public:

	HelpButton(Vector2 position, double radius);

	void renderImage(Renderer& renderer, Vector2 center, double radius) override;

	int getID() override;
};


class EditorButton : public MenuButton
{
public:

	EditorButton(Vector2 position, double radius);

	void renderImage(Renderer& renderer, Vector2 center, double radius) override;

	int getID() override;
};


class ExitButton : public MenuButton
{
public:

	ExitButton(Vector2 position, double radius);

	void renderImage(Renderer& renderer, Vector2 center, double radius) override;

	int getID() override;
};