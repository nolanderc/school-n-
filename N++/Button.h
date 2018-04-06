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

	// Knappens färg
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

	// Anger om knappen är markerad
	bool highlighted;

	// Anger hur 'markerad' knappen är
	float highlightAmount;

public:

	Button(int x, int y, int width, int height, std::string text);
	Button(RECT rect, std::string text);


	// Uppdatera knappen
	void update(float deltaTime);


	// Ändra knappens text
	void setText(std::string text);

	// Returnera knappens text
	std::string getText();


	// Sätt knappens färg
	void setColor(int r, int g, int b);

	// Sätt knappens färg när den är markerad
	void setColorHighlight(int r, int g, int b);


	// Sätter textens färg
	void setTextColor(int r, int g, int b);

	// Sätt textens färg när den är markerad
	void setTextColorHighlight(int r, int g, int b);


	// Anger om knappen är markerad eller inte
	void setHighlight(bool highlight);

	// Avgör om knappen är markerad
	bool isHighlighted();


	// Rita knappen
	void draw(Renderer& renderer);


	// Avgör om knappen innehåller en punkt
	bool contains(int x, int y);


	// Returnerar denna knapps id
	int getID();

	// Avgör om denna knapp har ett specifikt id
	bool operator==(int id);

private:

	// Returnera den markerade versionen av rektangeln
	RECT getHighlightedRectangle();

};
