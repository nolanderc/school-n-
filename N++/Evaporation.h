#pragma once
#include "Effect.h"
#include "NumberUtil.h"

class Evaporation : public Effect
{
	struct Stream {
		Vector2 base;
		double length;

		double distance;
		double speed;

		double age;
		double lifetime;

		double brightness;
	};


	double age;

	const double LIFETIME;

	std::vector<Stream> streams;

public:

	Evaporation(Vector2 center, double width, double height);

	bool isAlive() override;

	void update(double deltaTime, InteractionHandler* handler) override;

	void render(Renderer& renderer, Color background) override;
};
