#pragma once
#include "Renderer.h"
#include "ConvexHull.h"

#include "Interpolate.h"


class MenuButton
{
	// Knappens storlek och position
	Vector2 center;
	double radius;
	double actualRadius;

	// Antalet hörn
	int corners;

	// Formen av knappens bakgrund, används för att bestämma om knappen trycks av musen
	ConvexHull container;
	
	// Är knappen markerad?
	bool highlighted;
	
	// Hur markerad är knappen?
	double highlightAmount;


	// Är knappen vald?
	bool selected;

	// Hur markerad är knappen?
	double selectionAmount;


	// Knappens färg när den är markerad och inte markerad
	Color color, highlightColor;


public:

	virtual ~MenuButton() = default;

	// Skapar en ny knapp med en viss storlek och antal hörn
	MenuButton(Vector2 position, double radius, int corners);


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

	// Trycker eller släpper knappen
	void setSelected(bool selected);


	// Returnerar knappens ID
	virtual int getID() = 0;

private:

	// Skapar knappens kollisionsskal med en viss storlek
	ConvexHull createHull(double radius);
};
