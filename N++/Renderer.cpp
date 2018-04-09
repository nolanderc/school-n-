#include "Renderer.h"


Renderer::Renderer(HDC target, SIZE targetSize)
{
	this->target = target;
	this->targetSize = targetSize;

	this->currentPen = defaultPen();
	this->oldPen = (HPEN)SelectObject(this->target, this->currentPen);
	this->penStyle = LineStyle::LINE_SOLID;

	this->currentBrush = defaultBrush();
	this->oldBrush = (HBRUSH)SelectObject(this->target, this->currentBrush);
	this->brushStyle = FillStyle::FILL_SOLID;

	this->coordScale = 1;
}

Renderer Renderer::createBitmapRenderer(Bitmap& bitmap)
{
	HDC dc = CreateCompatibleDC(this->target);
	bitmap.selectInto(dc);

	return Renderer(dc, bitmap.getSize());
}

Renderer::~Renderer()
{
	this->switchPen(this->oldPen);
	this->switchBrush(this->oldBrush);
}

HDC Renderer::releaseDC()
{
	HDC dc = this->target;
	this->target = NULL;
	return dc;
}

void Renderer::scale(double scale)
{
	this->coordScale *= scale;
}

void Renderer::offset(Vector2 offset)
{
	this->coordOffset += offset;
}

void Renderer::setColor(int r, int g, int b)
{
	this->currentColor = RGB(r, g, b);
	this->switchPen(this->createCurrentPen());
}

void Renderer::setFillColor(int r, int g, int b)
{
	this->currentFillColor = RGB(r, g, b);
	this->switchBrush(this->createCurrentBrush());
}

void Renderer::setFillColor(COLORREF color)
{
	this->currentFillColor = color;
}

void Renderer::setLineStyle(LineStyle style)
{
	this->penStyle = style;
	this->switchPen(this->createCurrentPen());
}

void Renderer::setFillStyle(FillStyle style)
{
	this->brushStyle = style;
	this->switchBrush(this->createCurrentBrush());
}

void Renderer::setTextColor(int r, int g, int b)
{
	this->characterColors.clear();
	this->characterColors.push_back(RGB(r, g, b));
}

void Renderer::setTextColor(std::vector<COLORREF> colors)
{
	this->characterColors = colors;
}

void Renderer::setTextBackgroundColor(int r, int g, int b)
{
	SetBkColor(this->target, RGB(r, g, b));
}

void Renderer::setLineWidthAbsolute(int width)
{
	this->currentLineWidth = width;
	this->switchPen(this->createCurrentPen());
}

void Renderer::setLineWidth(double width)
{
	this->currentLineWidth = round(this->coordScale * width);
	this->switchPen(this->createCurrentPen());
}

void Renderer::clear()
{
	this->fillRect(0, 0, this->targetSize.cx, this->targetSize.cy);
}

void Renderer::drawLine(double x1, double y1, double x2, double y2)
{
	Vector2i start = this->transform({ x1, y1 });
	Vector2i end = this->transform({ x2, y2 });

	MoveToEx(this->target, start.x, start.y, NULL);
	LineTo(this->target, end.x, end.y);
}

void Renderer::drawLine(Vector2 start, Vector2 end)
{
	this->drawLine(start.x, start.y, end.x, end.y);
}

void Renderer::fillRect(double x, double y, double width, double height)
{
	Vector2i topLeft = this->transform({ x, y });
	Vector2i bottomRight = this->transform({ x + width, y + height });

	RECT rect;
	rect.left = topLeft.x;
	rect.right = bottomRight.x;
	rect.top = topLeft.y;
	rect.bottom = bottomRight.y;

	this->fillRect(rect);
}

void Renderer::fillRect(RECT rect)
{
	FillRect(this->target, &rect, this->currentBrush);
}

void Renderer::drawRect(double x, double y, double width, double height)
{
	drawLine(x, y, x + width, y);
	drawLine(x + width, y, x + width, y + height);
	drawLine(x + width, y + height, x, y + height);
	drawLine(x, y + height, x, y);
}

void Renderer::drawRect(RECT rect)
{
	drawLine(rect.left, rect.top, rect.right, rect.top);
	drawLine(rect.left, rect.bottom, rect.right, rect.bottom);
	drawLine(rect.left, rect.top, rect.left, rect.bottom);
	drawLine(rect.right, rect.top, rect.right, rect.bottom);
}

void Renderer::drawCircle(double x, double y, double radius)
{
	Vector2i topLeft = this->transform({ x - radius, y - radius });
	Vector2i bottomRight = this->transform({ x + radius, y + radius });

	Arc(this->target, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, 0, 0, 0, 0);
}

void Renderer::fillCircle(double x, double y, double radius)
{
	Vector2i topLeft = this->transform({ x - radius, y - radius });
	Vector2i bottomRight = this->transform({ x + radius, y + radius });

	Ellipse(this->target, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
}

void Renderer::fillPolygon(std::vector<POINT> points)
{
	Polygon(this->target, points.data(), points.size());
}

void Renderer::fillPolygon(std::vector<Vector2> points)
{
	std::vector<POINT> newPoints;

	int pointCount = points.size();
	for (int i = 0; i < pointCount; i++) {
		Vector2i point = this->transform(points[i]);
		newPoints.push_back({ point.x, point.y });
	}

	this->fillPolygon(newPoints);
}

void Renderer::drawPolygon(std::vector<POINT> points)
{
	points.push_back(points[0]);
	Polyline(this->target, points.data(), points.size());
}

void Renderer::drawPolygon(std::vector<Vector2> points)
{
	std::vector<POINT> newPoints;

	int pointCount = points.size();
	for (int i = 0; i < pointCount; i++) {
		Vector2i point = this->transform(points[i]);
		newPoints.push_back({ point.x, point.y });
	}

	this->drawPolygon(newPoints);
}

void Renderer::drawTextLeftAligned(std::string text, RECT rect)
{
	std::vector<int> characterSizes;
	int totalWidth = 0;
	int maxHeight = 0;

	int characterCount = text.size();
	for (int i = 0; i < characterCount; i++)
	{
		// Hämta textens storlek
		SIZE textSize;
		GetTextExtentPoint(this->target, &text[i], 1, &textSize);

		characterSizes.push_back(textSize.cx);
		totalWidth += textSize.cx;

		if (textSize.cy > maxHeight) {
			maxHeight = textSize.cy;
		}
	}

	// Anpassa rektangeln efter textens storlek
	rect.top = (rect.bottom + rect.top - maxHeight) / 2;

	int colorCount = this->characterColors.size();
	for (int i = 0; i < characterCount; i++)
	{
		// Sätt textens färg
		if (colorCount > 0) {
			SetTextColor(this->target, this->characterColors[i % colorCount]);
		}

		int size = characterSizes[i];
		rect.right = rect.left + size;
		DrawText(this->target, &text[i], 1, &rect, DT_CENTER);
		rect.left += size;
	}
}

void Renderer::drawTextCentered(std::string text, RECT rect)
{
	std::vector<int> characterSizes;
	int totalWidth = 0;
	int maxHeight = 0;

	int characterCount = text.size();
	for (int i = 0; i < characterCount; i++)
	{
		// Hämta textens storlek
		SIZE textSize;
		GetTextExtentPoint(this->target, &text[i], 1, &textSize);

		characterSizes.push_back(textSize.cx);
		totalWidth += textSize.cx;

		if (textSize.cy > maxHeight) {
			maxHeight = textSize.cy;
		}
	}

	// Anpassa rektangeln efter textens storlek
	rect.top = (rect.bottom + rect.top - maxHeight) / 2;
	rect.left = (rect.left + rect.right - totalWidth) / 2;

	int colorCount = this->characterColors.size();
	for (int i = 0; i < characterCount; i++)
	{
		// Sätt textens färg
		if (colorCount > 0) {
			SetTextColor(this->target, this->characterColors[i % colorCount]);
		}

		int size = characterSizes[i];
		rect.right = rect.left + size;
		DrawText(this->target, &text[i], 1, &rect, DT_CENTER);
		rect.left += size;
	}
}

void Renderer::drawTextCentered(std::string text, double left, double right, double top, double bottom)
{
	Vector2i topLeft = this->transform({ left, top });
	Vector2i bottomRight = this->transform({ right, bottom });

	RECT rect;
	rect.left = topLeft.x;
	rect.right = bottomRight.x;
	rect.top = topLeft.y;
	rect.bottom = bottomRight.y;

	this->drawTextCentered(text, rect);
}

void Renderer::drawBitmap(Bitmap& bitmap, int x, int y, int w, int h, int srcX, int srcY)
{
	HDC bitmapDC = CreateCompatibleDC(this->target);
	HBITMAP old = bitmap.selectInto(bitmapDC);
	BitBlt(this->target, x, y, w, h, bitmapDC, srcX, srcY, SRCCOPY);
	SelectObject(bitmapDC, old);
	DeleteDC(bitmapDC);
}

void Renderer::drawBitmapTransparent(Bitmap & bitmap, int x, int y, int w, int h, int srcX, int srcY, int srcW, int srcH, int filterR, int filterG, int filterB)
{
	HDC bitmapDC = CreateCompatibleDC(this->target);
	HBITMAP old = bitmap.selectInto(bitmapDC);
	TransparentBlt(this->target, x, y, w, h, bitmapDC, srcX, srcY, srcW, srcH, RGB(filterR, filterG, filterB));
	SelectObject(bitmapDC, old);
	DeleteDC(bitmapDC);
}

void Renderer::blitResult(HDC dc, int x, int y, int w, int h)
{
	BitBlt(dc, x, y, w, h, this->target, 0, 0, SRCCOPY);
}

void Renderer::blitResult(Bitmap bitmap, int x, int y, int w, int h)
{
}

HPEN Renderer::defaultPen()
{
	this->currentColor = RGB(0, 0, 0);
	this->currentLineWidth = 1;

	return this->createCurrentPen();
}

HBRUSH Renderer::defaultBrush()
{
	this->currentFillColor = RGB(0, 0, 0);
	return this->createCurrentBrush();
}

HPEN Renderer::createCurrentPen()
{
	return CreatePen(this->penStyle, this->currentLineWidth, this->currentColor);
}

HBRUSH Renderer::createCurrentBrush()
{
	switch (brushStyle)
	{
	case FILL_SOLID:
		return CreateSolidBrush(this->currentFillColor);
	case FILL_DIAGONAL_CROSS:
		return CreateHatchBrush(HS_DIAGCROSS, this->currentFillColor);
	default:
		break;
	}

}

void Renderer::switchPen(HPEN pen)
{
	SelectObject(this->target, pen);
	DeleteObject(this->currentPen);
	this->currentPen = pen;
}

void Renderer::switchBrush(HBRUSH brush)
{
	SelectObject(this->target, brush);
	DeleteObject(this->currentBrush);
	this->currentBrush = brush;
}

Vector2i Renderer::transform(Vector2 coord)
{
	return Vector2i(
		round((coord.x + this->coordOffset.x) * this->coordScale),
		round((coord.y + this->coordOffset.y) * this->coordScale)
	);
}
