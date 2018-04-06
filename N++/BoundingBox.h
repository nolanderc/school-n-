#pragma once
#include "Linear.h"

struct BoundingBox
{
	double left, right, top, bottom;

	BoundingBox();

	BoundingBox(double left, double right, double top, double bottom);

	// Returnerar true om två rektanglar kolliderar
	bool intersects(const BoundingBox& other) const;

	// Förflyttar rektangeln med en viss mängd
	void translate(Vector2 amount);
};

