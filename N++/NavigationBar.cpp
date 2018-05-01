#include "NavigationBar.h"

NavigationBar::NavigationBar(double width, double height, NavigationCallback* callback) :
	width(width), height(height), callback(callback)
{
}

void NavigationBar::render(Renderer& renderer, Color background)
{
	renderer.setFillColor(background);
	renderer.fillRect(0, 0, this->width, this->height);
	
	renderer.setTextBackgroundColor(background);

	int buttonCount = this->buttons.size();
	for (int i = 0; i < buttonCount; i++)
	{
		NavButton& button = this->buttons[i];

		renderer.setColor(Color(255));
		renderer.setLineStyle(LINE_SOLID);

		renderer.drawTextCentered("[" + getKeyName(button.key) + "] " + button.text, button.box);
	}
}

int NavigationBar::addButton(std::string text, KeyCode key)
{
	NavButton button;
	button.text = text;
	button.key = key;

	this->buttons.push_back(button);
	this->arrangeButtons();

	return this->buttons.size() - 1;
}

void NavigationBar::keyPressed(KeyCode key)
{
	if(this->callback)
	{
		int buttonCount = this->buttons.size();
		for (int i = 0; i < buttonCount; i++)
		{
			if (this->buttons[i].key == key) {
				this->callback->navigate(i);
				buttonCount = this->buttons.size();
			}
		}
	}
}

double NavigationBar::getWidth()
{
	return this->width;
}

double NavigationBar::getHeight()
{
	return this->height;
}

void NavigationBar::setWidth(double width)
{
	this->width = width;
	this->arrangeButtons();
}

void NavigationBar::setHeight(double height)
{
	this->height = height;
	this->arrangeButtons();
}

void NavigationBar::clear()
{
	this->buttons.clear();
}

void NavigationBar::arrangeButtons()
{
	int maxCharacterCount = 0;
	
	int buttonCount = this->buttons.size();
	for (int i = 0; i < buttonCount; i++)
	{
		NavButton& button = this->buttons[i];

		int count = button.text.size() + getKeyName(button.key).size();
		if (count > maxCharacterCount) {
			maxCharacterCount = count;
		}
	}


	double margin = 30;
	double pixelsPerCharacter = this->height / 4;
	double stride = maxCharacterCount * pixelsPerCharacter + margin;
	double totalWidth = stride * buttonCount - margin;

	BoundingBox box;
	box.left = (this->width - totalWidth) / 2;
	box.right = box.left;
	box.top = 0;
	box.bottom = this->height;

	for (int i = 0; i < buttonCount; i++)
	{
		NavButton& button = this->buttons[i];

		box.right = box.left + stride;
		button.box = box;

		box.left = box.right;
	}
}


std::string NavigationBar::getKeyName(KeyCode key)
{
	// Bokstäver
	if (KEY_A <= key && key <= KEY_Z) {
		std::string str;
		str.push_back(key - KEY_A + 'A');
		return str;
	}

	// Specialtangenter
	switch (key) {
	case KEY_ESCAPE: return "Escape";
	case KEY_SPACE:return "Space";
	case KEY_ENTER:return "Enter";
	case KEY_TAB:return "Tab";

	case KEY_UP:return "Up";
	case KEY_RIGHT:return "Right";
	case KEY_DOWN:return "Down";
	case KEY_LEFT:return "Left";

	case KEY_F1:return "F1";
	case KEY_F2:return "F2";
	case KEY_F3:return "F3";
	case KEY_F4:return "F4";
	case KEY_F5:return "F5";
	case KEY_F6:return "F6";
	case KEY_F7:return "F7";
	case KEY_F8:return "F8";
	case KEY_F9:return "F9";
	case KEY_F10:return "F10";
	case KEY_F11:return "F11";
	case KEY_F12:return "F12";

	default: return "Unknown";
	}
}

