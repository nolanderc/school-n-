#include "Evaporation.h"

Evaporation::Evaporation(Vector2 center, double width, double height) :
	LIFETIME(0.3)
{
	int streamCount = int(random(7, 20));
	for (int i = 0; i < streamCount; i++) {
		Stream stream;

		stream.base = center + Vector2(random(-width / 2, width / 2), random(-height / 2, height / 2));
		stream.length = random(0.1, 1.0);
		
		stream.distance = random(0, 0.1);
		stream.speed = random(4.0, 5.0);

		stream.age = 0;
		stream.lifetime = random(this->LIFETIME * 0.4, this->LIFETIME);

		stream.brightness = random(0, 1);

		this->streams.push_back(stream);
	}
}

bool Evaporation::isAlive()
{
	return this->age < this->LIFETIME;
}

void Evaporation::update(double deltaTime, InteractionHandler* handler)
{
	int streamCount = this->streams.size();
	for (int i = 0; i < streamCount; i++)
	{
		Stream& stream = this->streams[i];

		stream.distance += stream.speed * deltaTime;
		stream.age += deltaTime;

		if(stream.age > stream.lifetime) {
			this->streams.erase(this->streams.begin() + i);
			i--; streamCount--;
		}
	}
}

void Evaporation::render(Renderer& renderer, Color background)
{
	renderer.setLineWidthAbsolute(1);
	renderer.setLineStyle(LINE_SOLID);

	int streamCount = this->streams.size();
	for (int i = 0; i < streamCount; i++)
	{
		Stream& stream = this->streams[i];

		Vector2 end = stream.base - Vector2(0, stream.distance);
		Vector2 start = end + Vector2(0, stream.distance < stream.length ? stream.distance : stream.length);

		double rg = 0.8 + stream.brightness * 0.2;
		
		renderer.setColor(Color(rg, rg, stream.brightness));
		renderer.drawLine(start, end);
	}
}
