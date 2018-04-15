#include "MenuApp.h"

MenuApp::MenuApp() :
	App(512, 512, "N++")
{
	time = 0;

	this->buttons.push_back(new PlayButton({ 256 - 64, 256 }, 64));
	this->buttons.push_back(new EditorButton({ 256 + 64, 256 }, 64));
	this->buttons.push_back(new ExitButton({ 256, 256 + 128 * sqrt(1 - 0.5*0.5) }, 64));

	// this->addChild(new NinjaGame(this, "levels/customLevel.lvl"));
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
		if (menuButton->contains(Vector2(x, y))) {
			menuButton->setHighlight(false);
			menuButton->setSelected(false);

			switch (menuButton->getID())
			{
			case BUTTON_PLAY:
				this->addChild(new LevelSelector(this));
				break;

			case BUTTON_EDITOR:
				this->addChild(new LevelEditor(this));
				break;

			case BUTTON_EXIT:
				this->close();
				break;

			default: break;
			}
		}
	}
}
