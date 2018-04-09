#include "Interpolate.h"


double normalize(double value, double inMin, double inMax)
{
	return (clamp(value, inMin, inMax) - inMin) / (inMax - inMin);
}

double lmap(double value, double inMin, double inMax, double outMin, double outMax)
{
	return lerp(normalize(value, inMin, inMax), outMin, outMax);
}
