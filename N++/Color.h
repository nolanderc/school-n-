#pragma once

struct Color
{
	double r, g, b;

	Color(int grey);
	Color(double grey);
	Color(int r, int g, int b);
	Color(double r, double g, double b);

	// Låser alla färger inom intervallet [0, 1]
	void clampColors();

	// Blandar två färger
	Color mix(Color color, double amount = 0.5);
};
