#pragma once

#include <string>
#include "Renderer.h"


bool contains(RECT rect, int x, int y)
{
	return rect.left <= x && x <= rect.right
		&& rect.top <= y & y <= rect.bottom;
}


class Button {
	RECT rect;
	std::string text;

	// Knappens unika id
	int id;

	// Knappens f�rg
	struct Color {
		int r, g, b;

		Color(int r, int g, int b) : r(r), g(g), b(b) {}

		Color mix(const Color& other, float amount) {
			return Color(
				(other.r - this->r) * amount + this->r,
				(other.g - this->g) * amount + this->g,
				(other.b - this->b) * amount + this->b
			);
		}
	} color, highlightColor, textColor, textHighlightColor;
	float colorOffset = 0;

	// Anger om knappen �r markerad
	bool highlighted;

	// Anger hur 'markerad' knappen �r
	float highlightAmount;

public:

	Button(int x, int y, int width, int height, std::string text);
	Button(RECT rect, std::string text);


	// Uppdatera knappen
	void update(float deltaTime);


	// �ndra knappens text
	void setText(std::string text);

	// Returnera knappens text
	std::string getText();


	// S�tt knappens f�rg
	void setColor(int r, int g, int b);

	// S�tt knappens f�rg n�r den �r markerad
	void setColorHighlight(int r, int g, int b);


	// S�tter textens f�rg
	void setTextColor(int r, int g, int b);

	// S�tt textens f�rg n�r den �r markerad
	void setTextColorHighlight(int r, int g, int b);


	// Anger om knappen �r markerad eller inte
	void setHighlight(bool highlight);

	// Avg�r om knappen �r markerad
	bool isHighlighted();


	// Rita knappen
	void draw(Renderer& renderer);


	// Avg�r om knappen inneh�ller en punkt
	bool contains(int x, int y);


	// Returnerar denna knapps id
	int getID();

	// Avg�r om denna knapp har ett specifikt id
	bool operator==(int id);

private:

	// Returnera den markerade versionen av rektangeln
	RECT getHighlightedRectangle();

};
