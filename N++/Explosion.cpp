#include "Explosion.h"

Explosion::Explosion(Vector2 center) :
	center(center), age(0)
{
	enum ParticleType {
		FRAGMENT = 0,
		SMOKE = 1,
		FIRE = 2,
		TYPE_COUNT
	};


	int flameCount = random(100, 200);
	for (int i = 0; i < flameCount; i++)
	{
		Flame flame;
		flame.center = center;
		flame.distance = random(0.0, 1.0);
		flame.speed = random(16.0, 24.0);
		flame.direction = Vector2::rotated(random(0, 360));
		flame.lifetime = random(LIFETIME * 0.8, LIFETIME);

		int type;
		int r = rand() % 100;
		if (r < 20)
		{
			type = FRAGMENT;
		} else if (r < 60)
		{
			type = FIRE;
		} else
		{
			type = SMOKE;
		}

		switch (type) {
		case FRAGMENT:
			flame.width = random(0.05, 0.1);
			flame.maxLength = random(1, 2);

			flame.red = 255;
			flame.green = 50;
			flame.blue = 0;
			break;

		case SMOKE:
			flame.speed /= 2;

			flame.width = random(0.5, 1.0);
			flame.maxLength = random(0.5, 1);

			flame.red = 75;
			flame.green = 75;
			flame.blue = 75;
			break;

		case FIRE:
			flame.speed /= 2;

			flame.width = random(0.15, 0.3);
			flame.maxLength = random(0.5, 1.5);

			flame.red = 255;
			flame.green = 150;
			flame.blue = 0;
			break;

		default: break;
		}


		this->flames.push_back(flame);
	}
}

bool Explosion::isAlive()
{
	return this->age < LIFETIME;
}

void Explosion::update(double deltaTime)
{
	this->age += deltaTime;

	int count = this->flames.size();
	for (int i = 0; i < count; i++) {
		Flame& flame = this->flames[i];

		flame.distance += deltaTime * flame.speed;

		if (this->age > flame.lifetime) {
			this->flames.erase(this->flames.begin() + i);
			i--; count--;
		}
	}
}

void Explosion::render(Renderer& renderer)
{

	int count = this->flames.size();
	for (int i = 0; i < count; i++) {
		Flame& flame = this->flames[i];

		renderer.setColor(flame.red, flame.green, flame.blue);

		renderer.setLineWidth(lerp(this->age / flame.lifetime, flame.width, 0.0));

		Vector2 end = flame.center + flame.distance * flame.direction;
		Vector2 start = (flame.distance > flame.maxLength ? end - flame.maxLength * flame.direction : flame.center);

		renderer.drawLine(start, end);
	}
}
