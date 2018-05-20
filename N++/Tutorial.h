#pragma once

#include "App.h"
#include "Animation.h"
#include "ScrollUtil.h"

#include "NavigationBar.h"

class Tutorial: public App, public NavigationCallback
{
	struct Tip
	{
		Animation animation;
		std::string text;

		Tip(Animation animation, std::string text);
	};

	std::vector<Tip> animations;


	ScrollHelper scrollHelper;

	NavigationBar navBar;


public:
	Tutorial(App* parent);

protected:


	void update(double deltaTime) override;

	void draw(Renderer& renderer) override;


public:

	void keyPressed(KeyCode key) override;

	void mouseScrolled(int wheelDelta, int x, int y) override;

	void navigate(int id) override;

private:

	// Skapar alla animationer
	void createAnimations();

	// Skapar en animation med ninjan som går
	void createWalkingAnimation();
	
	// Skapar en animation med ninjan som hoppar
	void createJumpingAnimation();

	// Skapar en animation med ninjan som hoppar på vägger
	void createWallJumpingAnimation();

	// Skapar en animation med ninjan som glider på väggar och golv
	void createSpeedBoostingAnimation();

	// Skapar en animation med ninjan som klarar en nivå
	void createWinningAnimation();

	// Skapar en animation med ninjan som klarar en nivå genom att trycka på en knapp
	void createButtonAnimation();

	// Skapar en animation med ninjan som samlar mynt
	void createCoinsAnimation();

	// Skapar en animation med ninjan som misslyckas en nivå
	void createMinesAnimation();

	// Skapar en animation med ninjan som misslyckas en nivå
	void createRocketAnimation();
};
