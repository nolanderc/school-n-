#pragma once
#include "Bitmap.h"
#include "BoundingBox.h"
#include "Color.h"
#include <string>
#include <vector>

// Olika stilar på linjer
enum LineStyle
{
	LINE_NONE,
	LINE_SOLID,
};


// En klass som kan rita saker
class Renderer
{
public:

	virtual ~Renderer() = default;


	// Skapar en ny ritare som ritar till en bitmapsbild
	virtual Renderer* createBitmapRenderer(Bitmap* bitmap) = 0;


	// Anger hur mycket ska alla koordinater förstoras
	virtual void scale(double scale) = 0;

	// Anger hur mycket ska alla koordinater förflyttas
	virtual void offset(Vector2 offset) = 0;

	// Returnerar hur mycket koordinater förflyttas
	virtual Vector2 getOffset() = 0;


	// Sätter kanternas färg
	virtual void setColor(int r, int g, int b) = 0;
	virtual void setColor(Color color) = 0;

	// Sätter fyllningens färg
	virtual void setFillColor(int r, int g, int b) = 0;
	virtual void setFillColor(Color color) = 0;


	// Sätter textens färg
	virtual void setTextColor(int r, int g, int b) = 0;
	virtual void setTextColor(Color color) = 0;

	virtual void setTextBackgroundColor(int r, int g, int b) = 0;
	virtual void setTextBackgroundColor(Color color) = 0;

	// Sätter linjernas tjocklek, i pixlar
	virtual void setLineWidthAbsolute(int width) = 0;

	// Sätter linjernas tjocklek, i skalade pixlar
	virtual void setLineWidth(double width) = 0;


	// Sätter linjers stil
	virtual void setLineStyle(LineStyle style) = 0;


	// Rensar skärmen med den nuvarande färgen
	virtual void clear() = 0;


	// Ritar en linje ifrån (x1, y1) till (x2, y2)
	virtual void drawLine(double x1, double y1, double x2, double y2) = 0;
	virtual void drawLine(Vector2 start, Vector2 end) = 0;

	// Ritar en fylld rektangel
	virtual void fillRect(double x, double y, double width, double height) = 0;
	virtual void fillRect(BoundingBox box) = 0;

	// Ritar omkretsen av en rektangel
	virtual void drawRect(double x, double y, double width, double height) = 0;
	virtual void drawRect(BoundingBox box) = 0;

	// Ritar omkretsen av en cirkel
	virtual void drawCircle(double x, double y, double radius) = 0;


	// Ritar en fylld cirkel
	virtual void fillCircle(double x, double y, double radius) = 0;


	// Ritar en fylld polygon
	virtual void fillPolygon(std::vector<Vector2> points) = 0;

	// Ritar kanten av en polygon
	virtual void drawPolygon(std::vector<Vector2> points) = 0;


	// Ritar text till vänster innuti en rektangel
	virtual void drawTextLeftAligned(std::string text, BoundingBox box) = 0;

	// Ritar text centrerat inuti en rektangel
	virtual void drawTextCentered(std::string text, BoundingBox box) = 0;
	virtual void drawTextCentered(std::string text, double left, double right, double top, double bottom) = 0;

	// Ritar text till höger innuti en rektangel
	virtual void drawTextRightAligned(std::string text, BoundingBox box) = 0;


	// Ritar en bitmapsbild
	// Negativa tal för width och height ritar hela bilden
	virtual void drawBitmap(const Bitmap* bitmap, int x, int y, int width = -1, int height = -1,
	                        int srcX = 0, int srcY = 0) = 0;


	// Ritar en bitmapsbild
	virtual void drawBitmapTransparent(const Bitmap* bitmap, 
									   Color filterColor,
	                                   int x, int y, int width = -1, int height = -1,
	                                   int srcX = 0, int srcY = 0, int srcWidth = -1,
	                                   int srcHeight = -1) = 0;
};
