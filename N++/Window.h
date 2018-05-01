#pragma once
#include "Linear.h"
#include "Bitmap.h"
#include "Renderer.h"

// De olika musknapparna
enum MouseButton {
	MOUSE_LEFT,
	MOUSE_MIDDLE,
	MOUSE_RIGHT
};


// De olika tangenterna
enum KeyCode {
	// En okänd knapp
	KEY_UNKNOWN,

	// Specialtangenter
	KEY_ESCAPE,
	KEY_SPACE,
	KEY_ENTER,
	KEY_TAB,

	// Piltangenter
	KEY_UP,
	KEY_RIGHT,
	KEY_DOWN,
	KEY_LEFT,

	// Bokstäver
	KEY_A = 'A',
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,

	// Funktionsknappar
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12
};


// Hanterar meddelanden som kommer ifrån ett fönster
class WindowEventHandler {
public:
	virtual ~WindowEventHandler() = default;

	// Fönstret måste ritas om
	virtual void repaint() {};


	// Fönstret ändrade storlek
	virtual void sizeChanged(int width, int height) {}


	// En tangentbordsknapp trycktes
	virtual void keyPressed(KeyCode key) {}

	// En tangentbordsknapp släpptes
	virtual void keyReleased(KeyCode key) {}

	// En musknapp trycktes eller släpptes
	virtual void mousePressed(MouseButton button, int x, int y) {}
	virtual void mouseReleased(MouseButton button, int x, int y) {}

	// Muspekaren rörde sig
	virtual void mouseMoved(int x, int y) {}

	// Musens hjul rörde sig
	virtual void mouseScrolled(int wheelDelta, int x, int y) {}
};



// En klass som visar saker för och interagerar med användaren
class Window
{
public:

	virtual ~Window() = default;

	// Hanterar alla meddelanden som har anlänt sedan sita anropet av denna funktion
	virtual void pollMessages() = 0;

	// Sätter den klass som ska hantera fönstrets meddelanden
	virtual void setEventHandler(WindowEventHandler* handler) = 0;

	
	// Returnerar true om fönstret är öppet
	virtual bool isOpen() = 0;

	// Stänger fönstret
	virtual void close() = 0;



	// Returnerar fönstrets storlek
	virtual  Vector2i getSize() = 0;

	// Ändrar fönstrets storlek
	virtual void setSize(int width, int height) = 0;


	// Ändrar fönstrets rubrik/titelrad
	virtual void setTitle(std::string title) = 0;


	// Returnerar musens nuvarande position
	virtual Vector2i getMousePosition() = 0;


	// Avgör om en knapp är nedtryckt
	virtual bool isKeyDown(KeyCode key) = 0;


	// Skapar en bitmap kompatibel med fönstret
	virtual Bitmap* createCompatibleBitmap(Vector2i size) = 0;


	// Skapar en ny ritare som ritar till detta fönster
	virtual Renderer* getNewRenderer() = 0;

	// Avslutar ritningen och presenterar ritarens resultat i fönstret
	virtual void submitRenderer(Renderer& renderer) = 0;


	// Skickar ett varningsmeddelande till användaren
	virtual void alert(std::string title, std::string message) = 0;
};
