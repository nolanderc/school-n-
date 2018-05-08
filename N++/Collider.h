#pragma once

#include "ConvexHull.h"

// Representerar ett objekt som kan kollidera med andra objekt
class Collider
{
public:
	virtual ~Collider() = default;

	virtual Vector2* overlap(const ConvexHull& other) const = 0;

};



// Returerar den första kollisionen mellan ett objekt och en grupp objekt
Vector2* findFirstOverlap(const ConvexHull& hull, const std::vector<const Collider*>& colliders);
