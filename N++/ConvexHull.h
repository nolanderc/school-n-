#pragma once
#include <vector>

#include "Renderer.h"
#include "Linear.h"

#include "BoundingBox.h"

// En polygon
class ConvexHull
{
	// Alla punkter
	std::vector<Vector2> points;

	// Alla normaler
	std::vector<Vector2> normals;

	// En rektangel som omsluter alla punkter
	BoundingBox boundingBox;

public:

	ConvexHull();

	// Skapar ett konvext skal med punkter i klockvis ordning
	ConvexHull(std::vector<Vector2> points);

	

	// Fyller det konvexa skalet
	void fill(Renderer& renderer) const;

	// Ritar kanten av det konvexa skalet
	void draw(Renderer& renderer) const;


	// F�rflyttar alla punkter
	void translate(Vector2 amount);


	// Returnerar den genomsnittliga mittpunkten
	Vector2 average() const;


	// Returnerar en rektangel som omsluter detta skal
	BoundingBox getBoundingBox() const;


	// Returnerar en vektor som f�rflyttar tv� konvexa skal ur varandra
	Vector2* overlap(const ConvexHull& other) const;
};


// Projicerar flera punkter l�ngs med en axel, s�tter min och max till st�rsta och minsta v�rdena
void projectPoints(Vector2 axis, const std::vector<Vector2>& points, double* min, double* max);
