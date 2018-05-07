#include "ScrollUtil.h"

ScrollHelper::ScrollHelper(double maxScroll) :
	maxScroll(maxScroll), scrollAmount(0), scrollTarget(0)
{
}

void ScrollHelper::update(double deltaTime)
{
	double scrollDelta = this->scrollTarget - this->scrollAmount;

	double delta = abs(scrollDelta) < 1 ? scrollDelta : scrollDelta * deltaTime;

	this->scrollAmount += delta;
}

void ScrollHelper::addScroll(double amount)
{
	double scrollDelta = this->scrollTarget - this->scrollAmount;

	if (sign(scrollDelta) == sign(amount)) {
		this->scrollTarget += amount;
	}
	else {
		this->scrollTarget = this->scrollAmount + amount;
	}

	this->scrollTarget = clamp(
		this->scrollTarget,
		0.0,
		this->maxScroll
	);
}

void ScrollHelper::setMaxScroll(double maxScroll)
{
	this->maxScroll = maxScroll;
}

double ScrollHelper::getDistance()
{
	return this->scrollAmount;
}
