#pragma once
#include <Windows.h>
#include <string>
#include <vector>

#include "Bitmap.h"
#include "Linear.h"

// Olika stilar p� linjer
enum LineStyle {
	LINE_NONE = PS_NULL,
	LINE_SOLID = PS_SOLID,
};

// Olika stilar p� fyllningar
enum FillStyle {
	FILL_SOLID,
	FILL_DIAGONAL_CROSS
};

// Hanterar ritning av objekt.
// Den ritar till en g�md 'bitmap' (eller bild) f�r att undvika
// att bilden blinkar p� grund av att den uppdateras f�r l�ngsamt.
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

	// Den f�rg som kanter ritas med nu
	COLORREF currentColor;

	// Den f�rg som fylls med nu
	COLORREF currentFillColor;

	// Den f�rg/f�rger som anv�nds f�r att rita text
	std::vector<COLORREF> characterColors;

	// Den tjocklek av linjer som anv�nds nu
	int currentLineWidth;

	// Hur mycket ska alla koordinater f�rstoras?
	double coordScale;

	// Pekare till f�nstrets storlek
	SIZE targetSize;

public:

	// Skapar en ny ritare ifr�n en HDC att rita till och storleken p� omr�det att rita
	Renderer(HDC target, SIZE targetSize);


	// Skapar en ny ritare som ritar till en bitmapsbild
	Renderer createBitmapRenderer(Bitmap& bitmap);


	// Raderar ritarens HDC
	~Renderer();

	// Avslutar ritningen genom att returnera den underliggande DC:n
	HDC releaseDC();

	// Anger hur mycket ska alla koordinater f�rstoras
	void scale(double scale);

	// S�tter kanternas f�rg
	void setColor(int r, int g, int b);

	// S�tter fyllningens f�rg
	void setFillColor(int r, int g, int b);
	void setFillColor(COLORREF color);


	// S�tter textens f�rg
	void setTextColor(int r, int g, int b);
	void setTextColor(std::vector<COLORREF> colors);
	void setTextBackgroundColor(int r, int g, int b);

	// S�tter linjernas tjocklek, i pixlar
	void setLineWidthAbsolute(int width);

	// S�tter linjernas tjocklek, i skalade pixlar
	void setLineWidth(double width);


	// S�tter linjers stil
	void setLineStyle(LineStyle style);


	// S�tter fyllnaders stil
	void setFillStyle(FillStyle style);


	// Rensar sk�rmen med den nuvarande f�rgen
	void clear();


	// Ritar en linje ifr�n (x1, y1) till (x2, y2)
	void drawLine(double x1, double y1, double x2, double y2);
	void drawLine(Vector2 start, Vector2 end);

	// Ritar en fylld rektangel
	void fillRect(double x, double y, double width, double height);
	void fillRect(RECT rect);

	// Ritar omkretsen av en rektangel
	void drawRect(double x, double y, double width, double height);
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


	// Ritar text till v�nster inuti en rektangel
	void drawTextLeftAligned(std::string text, RECT rect);

	// Ritar text centrerat inuti en rektangel
	void drawTextCentered(std::string text, RECT rect);
	void drawTextCentered(std::string text, double left, double right, double top, double bottom);

	// Ritar en bitmapsbild
	void drawBitmap(Bitmap& bitmap, int x, int y, int w, int h, int srcX, int srcY);

	
	// Ritar en bitmapsbild
	void drawBitmapTransparent(Bitmap & bitmap, int x, int y, int w, int h, int srcX, int srcY, int srcW, int srcH, int filterR, int filterG, int filterB);


	// Kopierar denna ritares resultat till en dc
	void blitResult(HDC dc, int x, int y, int w, int h);

	// Kopierar denna ritares resultat till en bitmap
	void blitResult(Bitmap bitmap, int x, int y, int w, int h);

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
};

