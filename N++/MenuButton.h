#pragma once
#include "Renderer.h"
#include "ConvexHull.h"

#include "Interpolate.h"


class MenuButton
{
	// Formen av knappens bakgrund, används för att bestämma om knappen trycks av musen
	ConvexHull container;
	
	// Är knappen markerad?
	bool highlighted;
	
	// Hur markerad är knappen?
	double highlightAmount;


	// Knappens färg när den är markerad och inte markerad
	struct Color {
		double r, g, b;
	} color, highlightColor;


public:
	
	// Skapar en ny knapp med en viss storlek och bild på en viss position
	MenuButton(Vector2 position, double radius);


	// Uppdaterar knappen
	void update(double deltaTime);

	// Ritar knappen
	void render(Renderer& renderer);

	// Ritar en bild på knappen
	virtual void renderImage(Renderer& renderer, Vector2 center, double radius) = 0;

	// Avgör om en punkt är innuti knappen
	bool contains(Vector2 point);


	// Markerar eller avmarkerar knappen
	void setHighlight(bool highlight);


	// Returnerar knappens ID
	virtual int getID() = 0;
};
