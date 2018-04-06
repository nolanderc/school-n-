﻿#include "MenuApp.h"

MenuApp::MenuApp() :
	App(512, 512, "N++ --- Main Menu")
{
	time = 0;

	this->buttons.push_back(new PlayButton({ 256, 256 }, 50));
	this->buttons.push_back(new ExitButton({ 256, 384 }, 50));
}

void MenuApp::update(float deltaTime)
{
	time += deltaTime;

	int buttonCount = this->buttons.size();
	for (int i = 0; i < buttonCount; i++)
	{
		this->buttons[i]->update(deltaTime);
	}
}

void MenuApp::draw(Renderer& renderer)
{
	renderer.setLineStyle(LINE_NONE);

	double r = 255 * normalize(sin(time), -1, 1);
	double g = 255 * normalize(cos(time), -1, 1);

	renderer.setFillColor(r, g, 50);
	renderer.clear();

	int buttonCount = this->buttons.size();
	for (int i = 0; i < buttonCount; i++)
	{
		this->buttons[i]->render(renderer);
	}
}

void MenuApp::keyPressed(int key)
{
	if (key == 'G') {
		this->addChild(new NinjaGame(this));
	}
}

void MenuApp::mouseMoved(int x, int y)
{
	int buttonCount = this->buttons.size();
	for (int i = 0; i < buttonCount; i++)
	{
		MenuButton* button = this->buttons[i];
		if (button->contains(Vector2(x, y)))
		{
			button->setHighlight(true);
		}
		else
		{
			button->setHighlight(false);
		}
	}

	
}

void MenuApp::mousePressed(MouseButton button, int x, int y)
{
	int buttonCount = this->buttons.size();
	for (int i = 0; i < buttonCount; i++)
	{
		MenuButton* menuButton = this->buttons[i];
		if (menuButton->contains(Vector2(x, y))) {
			menuButton->setHighlight(false);

			switch (i)
			{
			case BUTTON_PLAY:
				this->addChild(new NinjaGame(this));
				break;

			case BUTTON_EXIT:
				this->close();
				break;

			default: break;
			}
		}
	}
}
