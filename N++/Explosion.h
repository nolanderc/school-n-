#pragma once
#include "Effect.h"
#include "NumberUtil.h"

class Explosion: public Effect
{
	struct Flame {
		Vector2 center;
		Vector2 direction;

		double distance;
		double maxLength;
		double width;

		double lifetime;

		double speed;

		double red;
		double green;
		double blue;
	};


	Vector2 center;

	double age;
	double lifetime;

	std::vector<Flame> flames;

public:
	
	// Skapar en ny, slumpmässig, explosion
	Explosion(Vector2 center, double size);

	bool isAlive() override;

	void update(double deltaTime) override;

	void render(Renderer& renderer) override;
};
