#include "Interpolate.h"

double clamp(double value, double min, double max)
{
	if (value < min) {
		return min;
	}
	else if (value > max) {
		return max;
	}
	else {
		return value;
	}
}


double normalize(double value, double inMin, double inMax)
{
	return (clamp(value, inMin, inMax) - inMin) / (inMax - inMin);
}

double lmap(double value, double inMin, double inMax, double outMin, double outMax)
{
	return lerp(normalize(value, inMin, inMax), outMin, outMax);
}
