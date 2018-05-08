#pragma once
#include "Renderer.h"
#include "Window.h"

class NavigationCallback
{
public:
	virtual ~NavigationCallback() = default;

	// Anger att en navigeringsknapp har tryckts
	virtual void navigate(int id) = 0;
};



class NavigationBar: public WindowEventHandler
{
	struct NavButton
	{
		std::string text;
		KeyCode key;

		BoundingBox box;
	};


	// Alla knappar i menyn
	std::vector<NavButton> buttons;

	// Storleken
	double width, height;

	// Anropas när en knapp har aktiverats
	NavigationCallback* callback;

public:
	
	NavigationBar(double width, double height, NavigationCallback* callback);


	// Ritar menyn
	void render(Renderer& renderer, Color background);


	// Lägger till en knapp till menyn som aktiveras med en knapp, returnerar dess id
	int addButton(std::string text, KeyCode key);


	// En knapp trycktes
	void keyPressed(KeyCode key) override;


	// Returnera storleken
	double getWidth();
	double getHeight();

	// Sätter storleken
	void setWidth(double width);
	void setHeight(double height);

	// Tar bort alla knappar
	void clear();


private:

	void arrangeButtons();


	static std::string getKeyName(KeyCode key);

};
