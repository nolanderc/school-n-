﻿#include "MenuApp.h"

MenuApp::MenuApp() :
	App(512, 512, "N++"), selectedButton(nullptr)
{
	time = 0;

	this->buttons.push_back(new PlayButton({ 256 - 64, 256 }, 64));
	this->buttons.push_back(new EditorButton({ 256 + 64, 256 }, 64));
	this->buttons.push_back(new ExitButton({ 256, 256 + 128 * sqrt(1 - 0.5*0.5) }, 64));
}

void MenuApp::update(float deltaTime)
{
	time += deltaTime;

	if (this->selectedButton)
	{
		MenuButton* button = this->buttons[*this->selectedButton];
		button->setHighlight(true);
	}


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
	if (key == VK_RETURN || key == VK_SPACE)
	{
		if (this->selectedButton)
		{
			this->buttonPressed(*this->selectedButton);
		}
	}

	if (key == VK_DOWN || key == 'S')
	{
		this->changeSelected(2);
	}

	if (key == VK_LEFT || key == 'A')
	{
		this->changeSelected(0);
	}

	if (key == VK_RIGHT || key == 'D')
	{
		this->changeSelected(1);
	}
}

void MenuApp::mouseMoved(int x, int y)
{
	delete this->selectedButton;
	this->selectedButton = nullptr;

	int buttonCount = this->buttons.size();
	for (int i = 0; i < buttonCount; i++)
	{
		MenuButton* button = this->buttons[i];
		if (button->contains(Vector2(x, y)))
		{
			button->setHighlight(true);
			this->selectedButton = new int(i);
		}
		else
		{
			button->setHighlight(false);
			button->setSelected(false);
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
			menuButton->setSelected(true);
		}
	}
}

void MenuApp::mouseReleased(MouseButton button, int x, int y)
{
	int buttonCount = this->buttons.size();
	for (int i = 0; i < buttonCount; i++)
	{
		MenuButton* menuButton = this->buttons[i];
		if (menuButton->contains(Vector2i(x, y)))
		{
			this->buttonPressed(i);
		}
	}
}

void MenuApp::changeSelected(int index)
{
	if (0 <= index && index < this->buttons.size())
	{
		if (this->selectedButton)
		{
			MenuButton* button = this->buttons[*this->selectedButton];
			button->setHighlight(false);

			*this->selectedButton = index;
		}
		else 
		{
			this->selectedButton = new int(index);
		}
	}
}

void MenuApp::buttonPressed(int buttonIndex)
{
	MenuButton* menuButton = this->buttons[buttonIndex];

	menuButton->setHighlight(false);
	menuButton->setSelected(false);

	switch (menuButton->getID())
	{
	case BUTTON_PLAY:
		this->addChild(new LevelSelector(this));
		break;

	case BUTTON_EDITOR:
		this->addChild(new LevelEditor(this, "levels/level2.lvl"));
		break;

	case BUTTON_EXIT:
		this->close();
		break;

	default: break;
	}
}
