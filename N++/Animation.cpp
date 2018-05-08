#include "Animation.h"

Keyframe::Keyframe(double time, double duration, NinjaMovement command) :
	time(time), duration(duration), command(command)
{
}

Animation::Animation(Level level, std::vector<Keyframe> keyframes, double duration, bool loop) :
	level(level), frames(keyframes), duration(duration), time(0), loop(loop), accumulator(0)
{
}

void Animation::update(double deltaTime)
{
	this->accumulator += deltaTime;
	
	const double TIME_STEP = 1.0 / 240;

	while (this->accumulator > TIME_STEP)
	{
		if (this->accumulator > 1.0)
		{
			this->accumulator = 0;
		}


		this->time += TIME_STEP;
		if (this->time > this->duration) {
			if (this->loop) {
				this->time -= this->duration;
			}
			else {
				this->time = 0.0;
				this->level.reset();
			}
		}


		int frameCount = this->frames.size();
		for (int i = 0; i < frameCount; i++)
		{
			Keyframe& frame = this->frames[i];

			if (frame.time < this->time && this->time < frame.time + frame.duration)
			{
				this->level.moveNinja(frame.command);
			}
		}


		this->accumulator -= TIME_STEP;
		this->level.increaseEnergy(30.0);
		this->level.update(TIME_STEP);
	}
}

void Animation::render(Renderer& renderer, Color background)
{
	renderer.setFillColor(background);
	renderer.fillRect(0, 0, this->level.getWidth(), this->level.getHeight());

	this->level.renderStatic(renderer);
	this->level.renderDynamic(renderer, background);
}

int Animation::getWidth()
{
	return this->level.getWidth();
}

int Animation::getHeight()
{
	return this->level.getHeight();
}
