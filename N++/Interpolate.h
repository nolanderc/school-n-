#pragma once

// Returnera ett värde som inte är större eller mindre än ett intervall
double clamp(double value, double min, double max);

// Omvandla en parameter i intervallet [0, 1] till intervallet [min, max]
template<class T>
double lerp(double value, T min, T max) {
	return clamp(value, 0, 1) * (max - min) + min;
}


// Omvandla en parameter i intervallet [inMin, inMax] till intervallet [0, 1]
double normalize(double value, double inMin, double inMax);


// Omvandla en parameter i intervallet [inMin, inMax] till intervallet [outMin, outMax]
double lmap(double value, double inMin, double inMax, double outMin, double outMax);
