#include "Explosion.h"

Explosion::Explosion(Vector2 center, double size) :
	center(center), age(0)
{
	enum ParticleType {
		FRAGMENT = 0,
		SMOKE = 1,
		FIRE = 2,
		TYPE_COUNT
	};

	this->lifetime = random(0.2 * sqrt(size), 0.3 * sqrt(size));

	int flameCount = random(10 * size, 20 * size);
	for (int i = 0; i < flameCount; i++)
	{
		Flame flame;
		flame.center = center;
		flame.distance = random(0.0, 0.1);
		flame.speed = random(24.0, 48.0);
		flame.direction = Vector2::rotated(random(0, 360));
		flame.lifetime = random(lifetime * size * 0.08, lifetime);

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
			flame.speed /= 3;

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
	return this->age < lifetime;
}

void Explosion::update(double deltaTime, InteractionHandler* handler)
{
	this->age += deltaTime;

	// Antalet gånger att kolla efter kollisioner för varje flamma
	// Ökar precisionen av reflektioner
	int samples = 5;

	for (int i = 0; i < samples; i++)
	{
		int count = this->flames.size();
		for (int i = 0; i < count; i++) {
			Flame& flame = this->flames[i];

			flame.distance += (deltaTime / samples) * flame.speed;

			// Begränsa längden så att det inte flyger saker genom block :P
			// Allt detta hade blivit mycket bättre, lättare och mer fårståeligt om
			// jag hade gjort matten för att beräkna vart två linjer korsade varandra.
			// Det är däremot mycket arbete och det här fungerar helt okej
			double maxLength = clamp(flame.maxLength, 0.0, 0.9);

			Vector2 end = flame.center + flame.distance * flame.direction;
			Vector2 start = (flame.distance > maxLength ? end - maxLength * flame.direction : flame.center);

			ConvexHull hull = ConvexHull::newLine(start, end, flame.width);
			if (Vector2* overlap = handler->getBlockOverlap(hull)) {
				Vector2 intersection = end - 1.1 * *overlap - flame.width * overlap->normal();

				flame.center = intersection;
				flame.distance = 0;
				flame.direction = flame.direction.reflect(overlap->normal());

				delete overlap;
			}
			else if (this->age > flame.lifetime) {
				this->flames.erase(this->flames.begin() + i);
				i--; count--;
			}
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
		renderer.setLineStyle(LINE_SOLID);

		Vector2 end = flame.center + flame.distance * flame.direction;
		Vector2 start = (flame.distance > flame.maxLength ? end - flame.maxLength * flame.direction : flame.center);

		renderer.drawLine(start, end);
	}
}
