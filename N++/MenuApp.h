#pragma once
#include "App.h"
#include "Interpolate.h"

#include "NinjaGame.h"
#include "MenuButton.h"
#include "MenuButtons.h"

class MenuApp: public App
{

	double time;

	std::vector<MenuButton*> buttons;

public:

	MenuApp();

protected:

	void update(float deltaTime) override;
	void draw(Renderer& renderer) override;


	void keyPressed(int key) override;

	void mouseMoved(int x, int y) override;

	void mousePressed(MouseButton button, int x, int y) override;
};
