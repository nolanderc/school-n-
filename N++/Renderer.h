#pragma once
#include <Windows.h>
#include <string>
#include <vector>

#include "Bitmap.h"
#include "Linear.h"
#include "BoundingBox.h"

// Olika stilar på linjer
enum LineStyle {
	LINE_NONE = PS_NULL,
	LINE_SOLID = PS_SOLID,
};

// Olika stilar på fyllningar
enum FillStyle {
	FILL_SOLID,
	FILL_DIAGONAL_CROSS
};

// Hanterar ritning av objekt.
// Den ritar till en gömd 'bitmap' (eller bild) för att undvika
// att bilden blinkar på grund av att den uppdateras för långsamt.
// Bilden visas sedan genom att anropa funktionen 'blitResult'.
class Renderer
{
	// En DC att rita till
	HDC target;

	// Pennor
	HPEN oldPen;
	HPEN currentPen;

	// Pennans stil
	LineStyle penStyle;

	// Penslar
	HBRUSH oldBrush;
	HBRUSH currentBrush;

	// Penselns stil
	FillStyle brushStyle;

	// Den färg som kanter ritas med nu
	COLORREF currentColor;

	// Den färg som fylls med nu
	COLORREF currentFillColor;

	// Den färg/färger som används för att rita text
	std::vector<COLORREF> characterColors;

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
	Renderer(HDC target, SIZE targetSize);


	// Skapar en ny ritare som ritar till en bitmapsbild
	Renderer createBitmapRenderer(Bitmap& bitmap);


	// Raderar ritarens HDC
	~Renderer();

	// Avslutar ritningen genom att returnera den underliggande DC:n
	HDC releaseDC();

	// Anger hur mycket ska alla koordinater förstoras
	void scale(double scale);

	// Anger hur mycket ska alla koordinater förflyttas
	void offset(Vector2 offset);

	// Returnerar hur mycket koordinater förflyttas
	Vector2 getOffset();


	// Sätter kanternas färg
	void setColor(int r, int g, int b);

	// Sätter fyllningens färg
	void setFillColor(int r, int g, int b);
	void setFillColor(COLORREF color);


	// Sätter textens färg
	void setTextColor(int r, int g, int b);
	void setTextColor(std::vector<COLORREF> colors);
	void setTextBackgroundColor(int r, int g, int b);

	// Sätter linjernas tjocklek, i pixlar
	void setLineWidthAbsolute(int width);

	// Sätter linjernas tjocklek, i skalade pixlar
	void setLineWidth(double width);


	// Sätter linjers stil
	void setLineStyle(LineStyle style);


	// Sätter fyllnaders stil
	void setFillStyle(FillStyle style);


	// Rensar skärmen med den nuvarande färgen
	void clear();


	// Ritar en linje ifrån (x1, y1) till (x2, y2)
	void drawLine(double x1, double y1, double x2, double y2);
	void drawLine(Vector2 start, Vector2 end);

	// Ritar en fylld rektangel
	void fillRect(double x, double y, double width, double height);
	void fillRect(BoundingBox box);
	void fillRect(RECT rect);

	// Ritar omkretsen av en rektangel
	void drawRect(double x, double y, double width, double height);
	void drawRect(BoundingBox box);
	void drawRect(RECT rect);

	// Ritar omkretsen av en cirkel
	void drawCircle(double x, double y, double radius);


	// Ritar en fylld cirkel
	void fillCircle(double x, double y, double radius);


	// Ritar en fylld polygon
	void fillPolygon(std::vector<POINT> points);
	void fillPolygon(std::vector<Vector2> points);

	// Ritar kanten av en polygon
	void drawPolygon(std::vector<POINT> points);
	void drawPolygon(std::vector<Vector2> points);


	// Ritar text till vänster inuti en rektangel
	void drawTextLeftAligned(std::string text, RECT rect);

	// Ritar text centrerat inuti en rektangel
	void drawTextCentered(std::string text, RECT rect);
	void drawTextCentered(std::string text, BoundingBox box);
	void drawTextCentered(std::string text, double left, double right, double top, double bottom);

	// Ritar en bitmapsbild
	// Negativa tal för width och height ritar hela bilden
	void drawBitmap(Bitmap& bitmap, int x, int y, int width = -1, int height = -1, int srcX = 0, int srcY = 0);

	
	// Ritar en bitmapsbild
	void drawBitmapTransparent(Bitmap & bitmap, int filterR, int filterG, int filterB, int x, int y, int width = -1, int height = -1, int srcX = 0, int srcY = 0, int srcWidth = -1, int srcHeight = -1);


	// Kopierar denna ritares resultat till en dc
	void blitResult(HDC dc, int x, int y, int w, int h);

	// Kopierar denna ritares resultat till en bitmap
	void blitResult(Bitmap bitmap, int x, int y, int w, int h);

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
};

