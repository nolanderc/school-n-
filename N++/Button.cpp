#include "Button.h"

Button::Button(int x, int y, int width, int height, std::string text) :
	Button(RECT(), text)
{
	this->rect.left = x;
	this->rect.right = x + width;
	this->rect.top = y;
	this->rect.bottom = y + height;
}

Button::Button(RECT rect, std::string text) :
	color(200, 200, 200), 
	highlightColor(220, 220, 220),
	textColor(0, 0, 0),
	textHighlightColor(40, 40, 40)
{
	this->rect = rect;
	this->text = text;

	this->highlighted = false;
	this->highlightAmount = 0;

	this->colorOffset = rand() / 100.0;

	// Skapa ett unikt id för knappen
	static int ID = 1;
	this->id = ID;
	ID++;
}

void Button::update(float deltaTime)
{
	if (this->highlighted) {
		this->highlightAmount += deltaTime * 3.0;
		if (highlightAmount > 1.0) {
			highlightAmount = 1.0;
		}
	}
	else {
		this->highlightAmount -= deltaTime * 6.0;
		if (highlightAmount < 0.0) {
			highlightAmount = 0.0;
		}
	}

	this->colorOffset += deltaTime * 10;
}

void Button::setText(std::string text)
{
	this->text = text;
}

std::string Button::getText()
{
	return text;
}

void Button::setColor(int r, int g, int b)
{
	this->color = Color(r, g, b);
}

void Button::setColorHighlight(int r, int g, int b)
{
	this->highlightColor = Color(r, g, b);
}

void Button::setTextColor(int r, int g, int b)
{
	this->textColor = Color(r, g, b);
}

void Button::setTextColorHighlight(int r, int g, int b)
{
	this->textHighlightColor = Color(r, g, b);
}

void Button::setHighlight(bool highlight)
{
	this->highlighted = highlight;
}

bool Button::isHighlighted()
{
	return this->highlighted;
}

void Button::draw(Renderer & renderer)
{
	Color button = this->color.mix(this->highlightColor, this->highlightAmount);
	Color text = this->textColor.mix(this->textHighlightColor, this->highlightAmount);

	renderer.setColor(button.r, button.g, button.b);
	renderer.setTextBackgroundColor(button.r, button.g, button.b);

	int dr = 255 - text.r;
	int dg = 255 - text.g;
	int db = 255 - text.b;

	std::vector<COLORREF> colors;
	int colorCount = 8;
	for (int i = 0; i < colorCount; i++)
	{
		colors.push_back(RGB(
			text.r + (i) * dr / colorCount,
			text.g + (i) * dg / colorCount,
			text.b + (i) * db / colorCount
		));
	}

	int x = colorOffset;
	x = x % colors.size();

	for (int i = 0; i < x; i++)
	{
		/*COLORREF first = colors[0];
		colors.erase(colors.begin());
		colors.push_back(first);*/
		COLORREF last = colors.back();
		colors.erase(colors.end() - 1);
		colors.insert(colors.begin(), last);
	}

	renderer.setTextColor(colors);

	renderer.fillRect(this->getHighlightedRectangle());
	renderer.drawTextCentered(this->text, this->rect);
}

bool Button::contains(int x, int y)
{
	return ::contains(this->getHighlightedRectangle(), x, y);
}

int Button::getID()
{
	return this->id;
}

bool Button::operator==(int id)
{
	return this->id == id;
}

RECT Button::getHighlightedRectangle()
{
	RECT sized = this->rect;
	int scale = 5;
	sized.left -= this->highlightAmount * scale;
	sized.right += this->highlightAmount * scale;
	sized.top -= this->highlightAmount * scale;
	sized.bottom += this->highlightAmount * scale;

	return sized;
}
