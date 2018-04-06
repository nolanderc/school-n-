#include "ConvexHull.h"




ConvexHull::ConvexHull()
{
}

ConvexHull::ConvexHull(std::vector<Vector2> points)
{
	this->points = points;

	int pointCount = points.size();
	for (int i = 0; i < pointCount; i++) {
		int j = (i + 1) % pointCount;

		Vector2 delta = points[j] - points[i];
		Vector2 normal = delta.normal().perpCW();

		this->normals.push_back(normal);

		Vector2& point = points[i];

		if (point.x < boundingBox.left) { boundingBox.left = point.x; }
		if (point.x > boundingBox.right) { boundingBox.right = point.x; }
		if (point.y < boundingBox.top) { boundingBox.top = point.y; }
		if (point.y > boundingBox.bottom) { boundingBox.bottom = point.y; }
	}
}


void ConvexHull::fill(Renderer & renderer) const
{
	renderer.fillPolygon(this->points);
}

void ConvexHull::draw(Renderer & renderer) const
{
	renderer.drawPolygon(this->points);
}

void ConvexHull::translate(Vector2 amount)
{
	int pointCount = points.size();
	for (int i = 0; i < pointCount; i++) {
		points[i] += amount;
	}

	this->boundingBox.translate(amount);
}

Vector2 ConvexHull::average() const
{
	Vector2 sum;

	int pointCount = points.size();
	for (int i = 0; i < pointCount; i++) {
		sum += points[i];
	}

	return sum / pointCount;
}

BoundingBox ConvexHull::getBoundingBox() const
{
	return this->boundingBox;
}

Vector2* ConvexHull::overlap(const ConvexHull & other) const
{
	const ConvexHull& self = *this;

	if (!self.boundingBox.intersects(other.boundingBox)) {
		return nullptr;
	}

	int axesCount = self.normals.size() + other.normals.size();
	std::vector<Vector2> axes;
	axes.reserve(axesCount);
	axes.insert(axes.end(), self.normals.begin(), self.normals.end());
	axes.insert(axes.end(), other.normals.begin(), other.normals.end());

	Vector2* smallestOverlap = nullptr;

	for (int i = 0; i < axesCount; i++)
	{
		Vector2 axis = axes[i];

		double selfMin, selfMax;
		projectPoints(axis, self.points, &selfMin, &selfMax);
		
		double otherMin, otherMax;
		projectPoints(axis, other.points, &otherMin, &otherMax);

		if (selfMin < otherMax && otherMin < selfMax) {
			double left = selfMax - otherMin;
			double right = otherMax - selfMin;

			double depth = (left < right ? -left : right);
			Vector2 overlap = axis * depth;
			if (smallestOverlap) {
				if (overlap.length() < smallestOverlap->length()) {
					*smallestOverlap = overlap;
				}
			}
			else {
				smallestOverlap = new Vector2(overlap);
			}
		}
		else {
			delete smallestOverlap;
			return nullptr;
		}
	}

	return smallestOverlap;
}

void projectPoints(Vector2 axis, const std::vector<Vector2>& points, double * min, double * max)
{
	int pointCount = points.size();
	for (int i = 0; i < pointCount; i++)
	{
		double projection = axis.dot(points[i]);
		if (i == 0) {
			*min = *max = projection;
		}
		else {
			*min = min(*min, projection);
			*max = max(*max, projection);
		}
	}
}
