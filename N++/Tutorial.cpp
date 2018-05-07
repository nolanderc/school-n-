#include "Tutorial.h"

Tutorial::Tutorial(App* parent) :
	App(parent),
	scrollHelper(0.0)
{
	this->setWindowSize(900, 700);

	this->createAnimations();
}

void Tutorial::update(float deltaTime)
{
	int animationCount = this->animations.size();
	for (int i = 0; i < animationCount; i++)
	{
		Animation& animation = this->animations[i];

		animation.update(deltaTime);
	}

	this->scrollHelper.update(deltaTime * 10);
}

void Tutorial::draw(Renderer& renderer)
{
	renderer.setFillColor(25, 25, 25);
	renderer.clear();


	renderer.offset(Vector2(0, -this->scrollHelper.getDistance()));

	renderer.scale(32);
	renderer.offset(Vector2(1, 1));

	int animationCount = this->animations.size();
	for (int i = 0; i < animationCount; i++)
	{
		Animation& animation = this->animations[i];

		renderer.setFillColor(50, 50, 50);
		animation.render(renderer);

		renderer.offset(Vector2(0, animation.getHeight() + 1));
	}
}

void Tutorial::keyPressed(KeyCode key)
{
	if (key == KEY_ESCAPE)
	{
		this->close();
	}

	if (key == KEY_UP || key == KEY_W)
	{
		this->scrollHelper.addScroll(-100);
	}

	if (key == KEY_DOWN || key == KEY_S)
	{
		this->scrollHelper.addScroll(100);
	}
}

void Tutorial::mouseScrolled(int wheelDelta, int x, int y)
{
	this->scrollHelper.addScroll(wheelDelta * -100);
}

void Tutorial::createAnimations()
{
	this->createWalkingAnimation();
	this->createJumpingAnimation();
	this->createWallJumpingAnimation();

	this->createWinningAnimation();
	this->createButtonAnimation();

	this->createCoinsAnimation();
	this->createMinesAnimation();

	this->createRocketAnimation();


	// Beräkna den maximala längden det går att skrolla
	double animationHeight = 0;
	int animationCount = this->animations.size();
	for (int i = 0; i < animationCount - 2; i++)
	{
		animationHeight += 32 * (this->animations[i].getHeight() + 1);
	}

	this->scrollHelper.setMaxScroll(animationHeight);
}

void Tutorial::createWalkingAnimation()
{
	Level level = Level("levels/tutorials/walking.lvl", HARD);

	std::vector<Keyframe> frames;
	frames.push_back(Keyframe(0.0, 2.0, NINJA_RIGHT));
	frames.push_back(Keyframe(2.0, 2.0, NINJA_LEFT));

	this->animations.push_back(Animation(level, frames, 4.0, true));
}

void Tutorial::createJumpingAnimation()
{
	Level level = Level("levels/tutorials/walking.lvl", HARD);

	std::vector<Keyframe> frames;
	frames.push_back(Keyframe(0.0, 1.0, NINJA_JUMP));

	this->animations.push_back(Animation(level, frames, 4.0, true));
}

void Tutorial::createWallJumpingAnimation()
{
	Level level = Level("levels/tutorials/walking.lvl", HARD);

	std::vector<Keyframe> frames;
	frames.push_back(Keyframe(0.0, 2.0, NINJA_RIGHT));
	frames.push_back(Keyframe(1.0, 0.1, NINJA_JUMP));
	frames.push_back(Keyframe(1.1, 0.1, NINJA_CANCEL_JUMP));
	frames.push_back(Keyframe(2.0, 2.0, NINJA_LEFT));

	frames.push_back(Keyframe(3.0, 0.1, NINJA_JUMP));
	frames.push_back(Keyframe(3.1, 0.1, NINJA_CANCEL_JUMP));

	this->animations.push_back(Animation(level, frames, 6.0, true));
}

void Tutorial::createWinningAnimation()
{
	Level level = Level("levels/tutorials/goal.lvl", HARD);

	std::vector<Keyframe> frames;
	frames.push_back(Keyframe(0.0, 3.0, NINJA_RIGHT));

	this->animations.push_back(Animation(level, frames, 4.0, false));
}

void Tutorial::createButtonAnimation()
{
	Level level = Level("levels/tutorials/button.lvl", HARD);

	std::vector<Keyframe> frames;
	frames.push_back(Keyframe(0.0, 3.0, NINJA_RIGHT));

	this->animations.push_back(Animation(level, frames, 4.0, false));
}

void Tutorial::createCoinsAnimation()
{
	Level level = Level("levels/tutorials/coins.lvl", HARD);

	std::vector<Keyframe> frames;
	frames.push_back(Keyframe(0.0, 3.0, NINJA_RIGHT));

	this->animations.push_back(Animation(level, frames, 4.0, false));
}

void Tutorial::createMinesAnimation()
{
	Level level = Level("levels/tutorials/mines.lvl", HARD);

	std::vector<Keyframe> frames;
	frames.push_back(Keyframe(0.0, 2.1, NINJA_RIGHT));
	frames.push_back(Keyframe(1.9, 0.1, NINJA_JUMP));
	frames.push_back(Keyframe(2.0, 0.1, NINJA_CANCEL_JUMP));

	frames.push_back(Keyframe(3.0, 2.1, NINJA_LEFT));
	frames.push_back(Keyframe(3.5, 0.1, NINJA_JUMP));


	this->animations.push_back(Animation(level, frames, 8.0, false));
}

void Tutorial::createRocketAnimation()
{
	Level level = Level("levels/tutorials/rocket.lvl", EASY);

	std::vector<Keyframe> frames;
	frames.push_back(Keyframe(0.0, 1.0, NINJA_RIGHT));

	this->animations.push_back(Animation(level, frames, 3.5, false));
}
