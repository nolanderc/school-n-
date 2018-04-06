#include "Collider.h"

Vector2* findFirstOverlap(const ConvexHull & hull, const std::vector<const Collider*>& colliders)
{
	Vector2* firstOverlap = nullptr;
	double smallestDepth = INFINITY;
	int colliderCount = colliders.size();
	for (int i = 0; i < colliderCount; i++)
	{
		if (Vector2* overlap = colliders[i]->overlap(hull)) {
			double depth = overlap->length();
			if (depth < smallestDepth) {
				delete firstOverlap;
				smallestDepth = depth;
				firstOverlap = overlap;
			}
			else {
				delete overlap;
			}
		}
	}

	return firstOverlap;
}
