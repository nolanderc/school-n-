#pragma once
#include "Linear.h"

struct BoundingBox
{
	double left, right, top, bottom;

	BoundingBox();

	BoundingBox(double left, double right, double top, double bottom);

	// Returnerar true om tv� rektanglar kolliderar
	bool intersects(const BoundingBox& other) const;

	// F�rflyttar rektangeln med en viss m�ngd
	void translate(Vector2 amount);

	// Avg�r om en punkt finns innuti rektangeln
	bool contains(Vector2 point);
};

