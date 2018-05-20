#pragma once
#include "App.h"
#include "Interpolate.h"

#include "MenuButtons.h"

#include "NinjaGame.h"
#include "Tutorial.h"
#include "LevelEditor.h"
#include "LevelSelector.h"


class MenuApp: public App
{
	// Hur många sekunder menyn har varit öppen
	double time;

	// Alla knappar på huvudmenyn
	std::vector<MenuButton*> buttons;

	// Den knapp som är markerad
	int* selectedButton;

	// Färgen på hemskärmens logo
	Color logoColors[2];

	// Hur lång tid som har passerat sedan senaste gången färgen bytts
	double colorChangeCooldown;

	// Hur ofta som hemskärmens logo snurrar
	const double SPIN_DURATION = 2.0;

	// Hur mycket hemskärmens logo ska blanda dess två färger
	double mixAmount;

public:

	// Skapar en meny i ett fönster
	MenuApp(Window* window);

	~MenuApp();

protected:

	// Uppdatera allt
	void update(double deltaTime) override;

	// Rita allt
	void draw(Renderer& renderer) override;

	// En knapp trycktes
	void keyPressed(KeyCode key) override;

	// Muspekaren rördes
	void mouseMoved(int x, int y) override;

	// En musknapp trycktes/släpptes
	void mousePressed(MouseButton button, int x, int y) override;
	void mouseReleased(MouseButton button, int x, int y) override;

private:

	// Ändrar den valda knappen
	void changeSelected(int index);


	// Anropas när en knapp trycks
	void buttonPressed(int buttonIndex);


	// Ritar en logo
	void drawLogo(Renderer& renderer, Color color);
};
