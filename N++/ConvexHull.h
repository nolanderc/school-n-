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

	// Skapar ett nytt konvext skal i form av en linje mellan två punkter med en viss vidd
	static ConvexHull newLine(Vector2 start, Vector2 end, double width);


	// Fyller det konvexa skalet
	void fill(Renderer& renderer) const;

	// Ritar kanten av det konvexa skalet
	void draw(Renderer& renderer) const;


	// Förflyttar alla punkter
	void translate(Vector2 amount);


	// Returnerar den genomsnittliga mittpunkten
	Vector2 average() const;


	// Returnerar en rektangel som omsluter detta skal
	BoundingBox getBoundingBox() const;


	// Returnerar en vektor som förflyttar två konvexa skal ur varandra
	Vector2* overlap(const ConvexHull& other) const;


	// Avgör om en punkt finns innuti detta skal
	bool contains(Vector2 point);
};


// Projicerar flera punkter längs med en axel, sätter min och max till största och minsta värdena
void projectPoints(Vector2 axis, const std::vector<Vector2>& points, double* min, double* max);
