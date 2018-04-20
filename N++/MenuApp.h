#pragma once
#include "App.h"
#include "Interpolate.h"

#include "MenuButtons.h"

#include "NinjaGame.h"
#include "LevelEditor.h"
#include "LevelSelector.h"


class MenuApp: public App
{
	double time;

	std::vector<MenuButton*> buttons;

	int* selectedButton;

public:

	MenuApp();

protected:

	void update(float deltaTime) override;
	void draw(Renderer& renderer) override;

	void keyPressed(int key) override;

	void mouseMoved(int x, int y) override;

	void mousePressed(MouseButton button, int x, int y) override;
	void mouseReleased(MouseButton button, int x, int y) override;

private:

	// Ändrar den valda knappen
	void changeSelected(int index);


	// Anropas när en knapp trycks
	void buttonPressed(int buttonIndex);
};
