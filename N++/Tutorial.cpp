#include "Tutorial.h"

Tutorial::Tip::Tip(Animation animation, std::string text) :
animation(animation), text(text)
{
}

Tutorial::Tutorial(App* parent) :
	App(parent),
	scrollHelper(0.0),
	navBar(1000, 32, this)
{
	this->setWindowSize(1000, 700);

	this->createAnimations();

	this->navBar.addButton("Back", KEY_ESCAPE);
}

void Tutorial::update(double deltaTime)
{
	int animationCount = this->animations.size();
	for (int i = 0; i < animationCount; i++)
	{
		Tip& tip = this->animations[i];

		tip.animation.update(deltaTime);
	}

	this->scrollHelper.update(deltaTime * 10);
}

void Tutorial::draw(Renderer& renderer)
{
	renderer.setFillColor(Color(25));
	renderer.setTextBackgroundColor(Color(25));
	renderer.clear();


	renderer.offset(Vector2(0, -this->scrollHelper.getDistance()));

	renderer.scale(32);
	renderer.offset(Vector2(1, 1));

	Vector2i windowSize = this->getWindowSize();

	int animationCount = this->animations.size();
	for (int i = 0; i < animationCount; i++)
	{
		Tip& tip = this->animations[i];

		tip.animation.render(renderer, Color(50));

		BoundingBox box;
		box.top = 0;
		box.bottom = tip.animation.getHeight();
		box.left = tip.animation.getWidth() + 1;
		box.right = windowSize.x / 32.0 - 1;

		renderer.setTextColor(Color(255));
		renderer.drawTextLeftAligned(tip.text, box);
		renderer.offset(Vector2(0, tip.animation.getHeight() + 1));
	}

	renderer.scale(1.0 / 32);
	renderer.offset(-renderer.getOffset());

	
	renderer.offset(Vector2(0, this->getWindowSize().y - this->navBar.getHeight()));
	this->navBar.render(renderer, Color(0));
}

void Tutorial::keyPressed(KeyCode key)
{
	this->navBar.keyPressed(key);
}

void Tutorial::mouseScrolled(int wheelDelta, int x, int y)
{
	this->scrollHelper.addScroll(wheelDelta * -100);
}

void Tutorial::navigate(int id)
{
	this->close();
}

void Tutorial::createAnimations()
{
	this->createWalkingAnimation();
	this->createJumpingAnimation();

	this->createWallJumpingAnimation();
	this->createSpeedBoostingAnimation();
	
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
		animationHeight += 32 * (this->animations[i].animation.getHeight() + 1);
	}

	this->scrollHelper.setMaxScroll(animationHeight);
}

void Tutorial::createWalkingAnimation()
{
	Level level = Level("levels/tutorials/walking.lvl", HARD);

	std::vector<Keyframe> frames;
	frames.push_back(Keyframe(0.0, 2.0, NINJA_RIGHT));
	frames.push_back(Keyframe(2.0, 2.0, NINJA_LEFT));

	this->animations.push_back(Tip(
		Animation(level, frames, 4.0, true),
		"Use the 'left and right arrow' keys to move left and right, respectively"
	));
}

void Tutorial::createJumpingAnimation()
{
	Level level = Level("levels/tutorials/walking.lvl", HARD);

	std::vector<Keyframe> frames;
	frames.push_back(Keyframe(0.0, 1.0, NINJA_JUMP));

	this->animations.push_back(Tip(
		Animation(level, frames, 4.0, true),
		"Press the 'up arrow' key to jump"
	));
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

	this->animations.push_back(Tip(
		Animation(level, frames, 6.0, true),
		"Move toward a wall to slide along it. Jump or press the 'down arrow' key to get off it"
	));
}

void Tutorial::createSpeedBoostingAnimation()
{
	Level level = Level("levels/tutorials/walking.lvl", HARD);

	std::vector<Keyframe> frames;
	frames.push_back(Keyframe(0.0, 2.5, NINJA_RIGHT));
	frames.push_back(Keyframe(1.9, 0.1, NINJA_JUMP));
	frames.push_back(Keyframe(2.0, 0.1, NINJA_CANCEL_JUMP));

	frames.push_back(Keyframe(3.5, 0.7, NINJA_LEFT));

	frames.push_back(Keyframe(4.0, 0.1, NINJA_JUMP));
	frames.push_back(Keyframe(4.1, 0.1, NINJA_CANCEL_JUMP));

	frames.push_back(Keyframe(5.5, 1.0, NINJA_LEFT));

	this->animations.push_back(Tip(
		Animation(level, frames, 7.0, true),
		"Save momentum by moving 'into' a wall or along the floor"
	));
}

void Tutorial::createWinningAnimation()
{
	Level level = Level("levels/tutorials/goal.lvl", HARD);

	std::vector<Keyframe> frames;
	frames.push_back(Keyframe(0.0, 3.0, NINJA_RIGHT));

	this->animations.push_back(Tip(
		Animation(level, frames, 4.0, false),
		"Get to the exit to win, the faster the better"
	));
}

void Tutorial::createButtonAnimation()
{
	Level level = Level("levels/tutorials/button.lvl", HARD);

	std::vector<Keyframe> frames;
	frames.push_back(Keyframe(0.0, 3.0, NINJA_RIGHT));

	this->animations.push_back(Tip(
		Animation(level, frames, 4.0, false),
		"Activate buttons to open doors"
	));
}

void Tutorial::createCoinsAnimation()
{
	Level level = Level("levels/tutorials/coins.lvl", HARD);

	std::vector<Keyframe> frames;
	frames.push_back(Keyframe(0.0, 3.0, NINJA_RIGHT));

	this->animations.push_back(Tip(
		Animation(level, frames, 4.0, false),
		"It's expensive to be a ninja, keep collecting coins or you will go bankrupt"
	));
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


	this->animations.push_back(Tip(
		Animation(level, frames, 7.0, false),
		"Mines aren't so nice, if you like being alive"
	));
}

void Tutorial::createRocketAnimation()
{
	Level level = Level("levels/tutorials/rocket.lvl", EASY);

	std::vector<Keyframe> frames;
	frames.push_back(Keyframe(0.0, 1.0, NINJA_RIGHT));

	this->animations.push_back(Tip(
		Animation(level, frames, 4.5, false),
		"Avoid heatseekers, they are deadly"
	));
}
