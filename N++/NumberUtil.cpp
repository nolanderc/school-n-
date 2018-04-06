#include "NumberUtil.h"

double random(double min, double max)
{
	return lerp(double(rand()) / RAND_MAX, min, max);
}
