#pragma once

// Returnera ett v�rde som inte �r st�rre eller mindre �n ett intervall
template <class T>
T clamp(T value, T min, T max) {
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

// Omvandla en parameter i intervallet [0, 1] till intervallet [min, max]
template<class T>
T lerp(double value, T min, T max) {
	return T(clamp(value, 0.0, 1.0)) * (max - min) + min;
}


// Omvandla en parameter i intervallet [0, 1] till intervallet [min, max]
// Detta sker "mjukare"/l�ngsammare n�rmare 0 och 1
template<class T>
T easeInOut(double value, T min, T max) {
	return lerp(3 * value*value - 2 * value*value*value, min, max);
}



// Omvandla en parameter i intervallet [inMin, inMax] till intervallet [0, 1]
double normalize(double value, double inMin, double inMax);


// Omvandla en parameter i intervallet [inMin, inMax] till intervallet [outMin, outMax]
double lmap(double value, double inMin, double inMax, double outMin, double outMax);


// Returnerar -1 f�r ett negativt tal, 1 f�r ett positivt tal och 0 f�r 0
double sign(double v);