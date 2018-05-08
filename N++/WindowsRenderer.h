#pragma once
#include <Windows.h>
#include <string>
#include <vector>

#include "Renderer.h"
#include "Linear.h"

#include "WindowsBitmap.h"

// Hanterar ritning av objekt.
// Den ritar till en g�md 'bitmap' (eller bild) f�r att undvika
// att bilden blinkar p� grund av att den uppdateras f�r l�ngsamt.
// Bilden visas sedan genom att anropa funktionen 'blitResult'.
class WindowsRenderer: public Renderer
{
	// En DC att rita till
	HDC target;

	// Pennor
	HPEN oldPen;
	HPEN currentPen;

	// Pennans stil
	int penStyle;

	// Penslar
	HBRUSH oldBrush;
	HBRUSH currentBrush;

	// Den f�rg som kanter ritas med nu
	COLORREF currentColor;

	// Den f�rg som fylls med nu
	COLORREF currentFillColor;

	// Den f�rg som anv�nds f�r att rita text
	COLORREF textColor;

	// Den tjocklek av linjer som anv�nds nu
	int currentLineWidth;

	// Hur mycket alla koordinater ska f�rstoras
	double coordScale;

	// Hur mycket alla koordinater ska f�rflyttas (sker f�re skalning)
	Vector2 coordOffset;

	// Pekare till f�nstrets storlek
	SIZE targetSize;

public:

	// Skapar en ny ritare ifr�n en HDC att rita till och storleken p� omr�det att rita
	WindowsRenderer(HDC target, SIZE targetSize);


	// Skapar en ny ritare som ritar till en bitmapsbild
	Renderer* createBitmapRenderer(Bitmap* bitmap) override;


	// Raderar ritarens HDC
	~WindowsRenderer();

	// Avslutar ritningen genom att returnera den underliggande DC:n
	HDC releaseDC();

	// Anger hur mycket ska alla koordinater f�rstoras
	void scale(double scale) override;

	// Anger hur mycket ska alla koordinater f�rflyttas
	void offset(Vector2 offset) override;

	// Returnerar hur mycket koordinater f�rflyttas
	Vector2 getOffset() override;


	// S�tter kanternas f�rg
	void setColor(int r, int g, int b) override;
	void setColor(Color color) override;

	// S�tter fyllningens f�rg
	void setFillColor(int r, int g, int b) override;
	void setFillColor(Color color) override;


	// S�tter textens f�rg
	void setTextColor(int r, int g, int b) override;
	void setTextColor(Color color) override;

	void setTextBackgroundColor(int r, int g, int b) override;
	void setTextBackgroundColor(Color color) override;

	// S�tter linjernas tjocklek, i pixlar
	void setLineWidthAbsolute(int width) override;

	// S�tter linjernas tjocklek, i skalade pixlar
	void setLineWidth(double width) override;


	// S�tter linjers stil
	void setLineStyle(LineStyle style) override;

	
	// Rensar sk�rmen med den nuvarande f�rgen
	void clear() override;


	// Ritar en linje ifr�n (x1, y1) till (x2, y2)
	void drawLine(double x1, double y1, double x2, double y2) override;
	void drawLine(Vector2 start, Vector2 end) override;

	// Ritar en fylld rektangel
	void fillRect(double x, double y, double width, double height) override;
	void fillRect(BoundingBox box) override;
	void fillRect(RECT rect);

	// Ritar omkretsen av en rektangel
	void drawRect(double x, double y, double width, double height) override;
	void drawRect(BoundingBox box) override;
	void drawRect(RECT rect);

	// Ritar omkretsen av en cirkel
	void drawCircle(double x, double y, double radius) override;


	// Ritar en fylld cirkel
	void fillCircle(double x, double y, double radius) override;


	// Ritar en fylld polygon
	void fillPolygon(std::vector<POINT> points);
	void fillPolygon(std::vector<Vector2> points) override;

	// Ritar kanten av en polygon
	void drawPolygon(std::vector<POINT> points);
	void drawPolygon(std::vector<Vector2> points) override;


	// Ritar text till v�nster innuti en rektangel
	void drawTextLeftAligned(std::string text, RECT rect);
	void drawTextLeftAligned(std::string text, BoundingBox box) override;
	
	// Ritar text centrerat inuti en rektangel
	void drawTextCentered(std::string text, RECT rect);
	void drawTextCentered(std::string text, BoundingBox box) override;
	void drawTextCentered(std::string text, double left, double right, double top, double bottom) override;

	// Ritar text till h�ger innuti en rektangel
	void drawTextRightAligned(std::string text, RECT rect);
	void drawTextRightAligned(std::string text, BoundingBox box) override;


	// Ritar en bitmapsbild
	// Negativa tal f�r width och height ritar hela bilden
	void drawBitmap(const Bitmap* bitmap, int x, int y, int width = -1, int height = -1, int srcX = 0, int srcY = 0) override;


	// Ritar en bitmapsbild
	void drawBitmapTransparent(const Bitmap* bitmap, Color filterColor, int x, int y, int width = -1, int height = -1, int srcX = 0, int srcY = 0, int srcWidth = -1, int srcHeight = -1) override;


	// Kopierar denna ritares resultat till en dc
	void blitResult(HDC dc, int x, int y, int w, int h);


	// Kopierar denna ritares resultat till en bitmap
	void blitResult(Bitmap* bitmap, int x, int y, int w, int h);

private:

	// Skapar default pennan
	HPEN defaultPen();

	// Skapar default penseln
	HBRUSH defaultBrush();

	// Skapar en ny penna utifr�n nuvarande v�rden p� f�rg och tjocklek
	HPEN createCurrentPen();

	// Skapar en ny pensel utifr�n nuvarande v�rden p� fyllf�rg
	HBRUSH createCurrentBrush();

	// Byter penna och raderar den gamla
	void switchPen(HPEN pen);

	// Byter pensel och raderar den gamla
	void switchBrush(HBRUSH brush);


	// Omvandlar en koordinat till sk�rmkoordinater (applicerar skalning och f�rflyttningar)
	Vector2i transform(Vector2 coord);


	// Omvandlar en f�rg till en f�rg i WINAPI
	COLORREF colorToColorref(Color color);
};

