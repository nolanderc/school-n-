#pragma once
#include <Windows.h>
#include <string>
#include <vector>

#include "Renderer.h"
#include "Linear.h"

#include "WindowsBitmap.h"

// Hanterar ritning av objekt.
// Den ritar till en gömd 'bitmap' (eller bild) för att undvika
// att bilden blinkar på grund av att den uppdateras för långsamt.
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

	// Den färg som kanter ritas med nu
	COLORREF currentColor;

	// Den färg som fylls med nu
	COLORREF currentFillColor;

	// Den färg som används för att rita text
	COLORREF textColor;

	// Den tjocklek av linjer som används nu
	int currentLineWidth;

	// Hur mycket alla koordinater ska förstoras
	double coordScale;

	// Hur mycket alla koordinater ska förflyttas (sker före skalning)
	Vector2 coordOffset;

	// Pekare till fönstrets storlek
	SIZE targetSize;

public:

	// Skapar en ny ritare ifrån en HDC att rita till och storleken på området att rita
	WindowsRenderer(HDC target, SIZE targetSize);


	// Skapar en ny ritare som ritar till en bitmapsbild
	Renderer* createBitmapRenderer(Bitmap* bitmap) override;


	// Raderar ritarens HDC
	~WindowsRenderer();

	// Avslutar ritningen genom att returnera den underliggande DC:n
	HDC releaseDC();

	// Anger hur mycket ska alla koordinater förstoras
	void scale(double scale) override;

	// Anger hur mycket ska alla koordinater förflyttas
	void offset(Vector2 offset) override;

	// Returnerar hur mycket koordinater förflyttas
	Vector2 getOffset() override;


	// Sätter kanternas färg
	void setColor(int r, int g, int b) override;
	void setColor(Color color) override;

	// Sätter fyllningens färg
	void setFillColor(int r, int g, int b) override;
	void setFillColor(Color color) override;


	// Sätter textens färg
	void setTextColor(int r, int g, int b) override;
	void setTextColor(Color color) override;

	void setTextBackgroundColor(int r, int g, int b) override;
	void setTextBackgroundColor(Color color) override;

	// Sätter linjernas tjocklek, i pixlar
	void setLineWidthAbsolute(int width) override;

	// Sätter linjernas tjocklek, i skalade pixlar
	void setLineWidth(double width) override;


	// Sätter linjers stil
	void setLineStyle(LineStyle style) override;

	
	// Rensar skärmen med den nuvarande färgen
	void clear() override;


	// Ritar en linje ifrån (x1, y1) till (x2, y2)
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


	// Ritar text till vänster innuti en rektangel
	void drawTextLeftAligned(std::string text, RECT rect);
	void drawTextLeftAligned(std::string text, BoundingBox box) override;
	
	// Ritar text centrerat inuti en rektangel
	void drawTextCentered(std::string text, RECT rect);
	void drawTextCentered(std::string text, BoundingBox box) override;
	void drawTextCentered(std::string text, double left, double right, double top, double bottom) override;

	// Ritar text till höger innuti en rektangel
	void drawTextRightAligned(std::string text, RECT rect);
	void drawTextRightAligned(std::string text, BoundingBox box) override;


	// Ritar en bitmapsbild
	// Negativa tal för width och height ritar hela bilden
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

	// Skapar en ny penna utifrån nuvarande värden på färg och tjocklek
	HPEN createCurrentPen();

	// Skapar en ny pensel utifrån nuvarande värden på fyllfärg
	HBRUSH createCurrentBrush();

	// Byter penna och raderar den gamla
	void switchPen(HPEN pen);

	// Byter pensel och raderar den gamla
	void switchBrush(HBRUSH brush);


	// Omvandlar en koordinat till skärmkoordinater (applicerar skalning och förflyttningar)
	Vector2i transform(Vector2 coord);


	// Omvandlar en färg till en färg i WINAPI
	COLORREF colorToColorref(Color color);
};

