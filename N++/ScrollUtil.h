#pragma once
#include "Interpolate.h"
#include "Linear.h"

class ScrollHelper
{
	// Hur långt som har skrollats
	double scrollAmount;

	// Hur långt som borde skrollats
	double scrollTarget;

	// Hur långt ner som man kan skrolla
	double maxScroll;


public:
	
	ScrollHelper(double maxScroll);


	void update(double deltaTime);


	// Anger att det borde skrollas visst mycket
	void addScroll(double amount);


	void setMaxScroll(double maxScroll);


	// Returnerar hur mycket som har skrollats
	double getDistance();
};
