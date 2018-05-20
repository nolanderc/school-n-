#pragma once

struct Color
{
	double r, g, b;

	// Skapa en svart färg
	Color();

	// Skapa en färg ifrån en gråskala [0, 255]
	Color(int grey);

	// Skapa en färg ifrån en gråskala [0, 1]
	Color(double grey);

	// Skapa en färg ifrån rgb-värden [0, 255]
	Color(int r, int g, int b);

	// Skapa en färg ifrån rgb-värden [0, 1]
	Color(double r, double g, double b);

	// Låser alla färger inom intervallet [0, 1]
	void clampColors();

	// Blandar två färger
	Color mix(Color color, double amount = 0.5);
};
