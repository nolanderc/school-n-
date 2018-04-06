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
	const double LIFETIME = 0.5;

	std::vector<Flame> flames;

public:
	
	// Skapar en ny, slumpmässig, explosion
	Explosion(Vector2 center);

	bool isAlive() override;

	void update(double deltaTime) override;

	void render(Renderer& renderer) override;
};
