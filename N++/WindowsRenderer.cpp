#include "WindowsRenderer.h"


WindowsRenderer::WindowsRenderer(HDC target, SIZE targetSize)
{
	this->target = target;
	this->targetSize = targetSize;

	this->currentPen = defaultPen();
	this->oldPen = (HPEN)SelectObject(this->target, this->currentPen);
	this->penStyle = LineStyle::LINE_SOLID;

	this->currentBrush = defaultBrush();
	this->oldBrush = (HBRUSH)SelectObject(this->target, this->currentBrush);

	this->coordScale = 1;
}

Renderer* WindowsRenderer::createBitmapRenderer(Bitmap* bitmap)
{
	WindowsBitmap* windowsBitmap = (WindowsBitmap*)bitmap;

	HDC dc = CreateCompatibleDC(this->target);
	windowsBitmap->selectInto(dc);

	return new WindowsRenderer(dc, windowsBitmap->getSize());
}

WindowsRenderer::~WindowsRenderer()
{
	this->switchPen(this->oldPen);
	this->switchBrush(this->oldBrush);

	if (this->target) {
		DeleteDC(this->target);
	}
}

HDC WindowsRenderer::releaseDC()
{
	HDC dc = this->target;
	this->target = nullptr;
	return dc;
}

void WindowsRenderer::scale(double scale)
{
	this->coordScale *= scale;
}

void WindowsRenderer::offset(Vector2 offset)
{
	this->coordOffset += offset * this->coordScale;
}

Vector2 WindowsRenderer::getOffset()
{
	return this->coordOffset;
}

void WindowsRenderer::setColor(int r, int g, int b)
{
	this->currentColor = RGB(r, g, b);
	this->switchPen(this->createCurrentPen());
}

void WindowsRenderer::setColor(Color color)
{
	color.clampColors();
	this->setColor(color.r * 255, color.g * 255, color.b * 255);
}

void WindowsRenderer::setFillColor(int r, int g, int b)
{
	this->currentFillColor = RGB(r, g, b);
	this->switchBrush(this->createCurrentBrush());
}

void WindowsRenderer::setFillColor(Color color)
{
	color.clampColors();
	this->setFillColor(color.r * 255, color.g * 255, color.b * 255);
}

void WindowsRenderer::setLineStyle(LineStyle style)
{
	switch (style)
	{
	case LINE_SOLID: this->penStyle = PS_SOLID; break;
	case LINE_NONE: this->penStyle = PS_NULL; break;
	default: ;
	}

	this->switchPen(this->createCurrentPen());
}


void WindowsRenderer::setTextColor(int r, int g, int b)
{
	this->textColor = RGB(r, g, b);
	SetTextColor(this->target, this->textColor);
}

void WindowsRenderer::setTextColor(Color color)
{
	color.clampColors();
	this->setTextColor(color.r * 255, color.g * 255, color.b * 255);
}

void WindowsRenderer::setTextBackgroundColor(int r, int g, int b)
{
	SetBkColor(this->target, RGB(r, g, b));
}

void WindowsRenderer::setTextBackgroundColor(Color color)
{
	color.clampColors();
	this->setTextBackgroundColor(color.r * 255, color.g * 255, color.b * 255);
}

void WindowsRenderer::setLineWidthAbsolute(int width)
{
	this->currentLineWidth = width;
	this->switchPen(this->createCurrentPen());
}

void WindowsRenderer::setLineWidth(double width)
{
	this->currentLineWidth = round(this->coordScale * width);
	this->switchPen(this->createCurrentPen());
}

void WindowsRenderer::clear()
{
	this->fillRect(0, 0, this->targetSize.cx, this->targetSize.cy);
}

void WindowsRenderer::drawLine(double x1, double y1, double x2, double y2)
{
	Vector2i start = this->transform({ x1, y1 });
	Vector2i end = this->transform({ x2, y2 });

	MoveToEx(this->target, start.x, start.y, NULL);
	LineTo(this->target, end.x, end.y);
}

void WindowsRenderer::drawLine(Vector2 start, Vector2 end)
{
	this->drawLine(start.x, start.y, end.x, end.y);
}

void WindowsRenderer::fillRect(double x, double y, double width, double height)
{
	Vector2i topLeft = this->transform({x, y});
	Vector2i bottomRight(
		int(ceil(width * this->coordScale) + topLeft.x),
		int(ceil(height * this->coordScale) + topLeft.y)
	);

	RECT rect;
	rect.left = topLeft.x;
	rect.right = bottomRight.x;
	rect.top = topLeft.y;
	rect.bottom = bottomRight.y;

	this->fillRect(rect);
}

void WindowsRenderer::fillRect(BoundingBox box)
{
	this->fillRect(box.left, box.top, box.right - box.left, box.bottom - box.top);
}

void WindowsRenderer::fillRect(RECT rect)
{
	FillRect(this->target, &rect, this->currentBrush);
}

void WindowsRenderer::drawRect(double x, double y, double width, double height)
{
	drawLine(x, y, x + width, y);
	drawLine(x + width, y, x + width, y + height);
	drawLine(x + width, y + height, x, y + height);
	drawLine(x, y + height, x, y);
}

void WindowsRenderer::drawRect(BoundingBox box)
{
	this->drawRect(box.left, box.top, box.right - box.left, box.bottom - box.top);
}

void WindowsRenderer::drawRect(RECT rect)
{
	drawLine(rect.left, rect.top, rect.right, rect.top);
	drawLine(rect.left, rect.bottom, rect.right, rect.bottom);
	drawLine(rect.left, rect.top, rect.left, rect.bottom);
	drawLine(rect.right, rect.top, rect.right, rect.bottom);
}

void WindowsRenderer::drawCircle(double x, double y, double radius)
{
	Vector2i topLeft = this->transform({ x - radius, y - radius });
	Vector2i bottomRight = this->transform({ x + radius, y + radius });

	Arc(this->target, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, 0, 0, 0, 0);
}

void WindowsRenderer::fillCircle(double x, double y, double radius)
{
	Vector2i topLeft = this->transform({ x - radius, y - radius });
	Vector2i bottomRight = this->transform({ x + radius, y + radius });

	Ellipse(this->target, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
}

void WindowsRenderer::fillPolygon(std::vector<POINT> points)
{
	Polygon(this->target, points.data(), points.size());
}

void WindowsRenderer::fillPolygon(std::vector<Vector2> points)
{
	std::vector<POINT> newPoints;

	int pointCount = points.size();
	for (int i = 0; i < pointCount; i++) {
		Vector2i point = this->transform(points[i]);
		newPoints.push_back({ point.x, point.y });
	}

	this->fillPolygon(newPoints);
}

void WindowsRenderer::drawPolygon(std::vector<POINT> points)
{
	points.push_back(points[0]);
	Polyline(this->target, points.data(), points.size());
}

void WindowsRenderer::drawPolygon(std::vector<Vector2> points)
{
	std::vector<POINT> newPoints;

	int pointCount = points.size();
	for (int i = 0; i < pointCount; i++) {
		Vector2i point = this->transform(points[i]);
		newPoints.push_back({ point.x, point.y });
	}

	this->drawPolygon(newPoints);
}

void WindowsRenderer::drawTextLeftAligned(std::string text, RECT rect)
{
	DrawText(this->target, text.c_str(), text.size(), &rect, DT_VCENTER | DT_SINGLELINE | DT_LEFT | DT_NOCLIP);
}

void WindowsRenderer::drawTextLeftAligned(std::string text, BoundingBox box)
{
	Vector2i topLeft = this->transform({ box.left, box.top });
	Vector2i bottomRight = this->transform({ box.right, box.bottom });

	RECT rect;
	rect.left = topLeft.x;
	rect.right = bottomRight.x;
	rect.top = topLeft.y;
	rect.bottom = bottomRight.y;

	this->drawTextLeftAligned(text, rect);
}

void WindowsRenderer::drawTextCentered(std::string text, RECT rect)
{
	DrawText(this->target, text.c_str(), text.size(), &rect, DT_VCENTER | DT_CENTER | DT_SINGLELINE | DT_NOCLIP);
}

void WindowsRenderer::drawTextCentered(std::string text, BoundingBox box)
{
	this->drawTextCentered(text, box.left, box.right, box.top, box.bottom);
}

void WindowsRenderer::drawTextCentered(std::string text, double left, double right, double top, double bottom)
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

void WindowsRenderer::drawTextRightAligned(std::string text, RECT rect)
{
	DrawText(this->target, text.c_str(), text.size(), &rect, DT_VCENTER | DT_SINGLELINE | DT_RIGHT | DT_NOCLIP);
}

void WindowsRenderer::drawTextRightAligned(std::string text, BoundingBox box)
{
	Vector2i topLeft = this->transform({ box.left, box.top });
	Vector2i bottomRight = this->transform({ box.right, box.bottom });

	RECT rect;
	rect.left = topLeft.x;
	rect.right = bottomRight.x;
	rect.top = topLeft.y;
	rect.bottom = bottomRight.y;

	this->drawTextRightAligned(text, rect);
}

void WindowsRenderer::drawBitmap(const Bitmap* bitmap, int x, int y, int width, int height, int srcX, int srcY)
{
	WindowsBitmap* windowsBitmap = (WindowsBitmap*)bitmap;

	HDC bitmapDC = CreateCompatibleDC(this->target);
	HBITMAP old = windowsBitmap->selectInto(bitmapDC);

	BitBlt(
		this->target,
		x, y,
		width < 0 ? windowsBitmap->getWidth() : width, height < 0 ? windowsBitmap->getHeight() : height,
		bitmapDC,
		srcX, srcY,
		SRCCOPY
	);

	SelectObject(bitmapDC, old);
	DeleteDC(bitmapDC);
}

void WindowsRenderer::drawBitmapTransparent(const Bitmap* bitmap, int filterR, int filterG, int filterB, int x, int y, int width, int height, int srcX, int srcY, int srcWidth, int srcHeight)
{
	WindowsBitmap* windowsBitmap = (WindowsBitmap*)bitmap;

	HDC bitmapDC = CreateCompatibleDC(this->target);
	HBITMAP old = windowsBitmap->selectInto(bitmapDC);

	TransparentBlt(
		this->target,
		x, y, 
		width < 0 ? windowsBitmap->getWidth() : width, height < 0 ? windowsBitmap->getHeight() : height,
		bitmapDC, 
		srcX, srcY, 
		srcWidth < 0 ? windowsBitmap->getWidth() : srcWidth, srcHeight < 0 ? windowsBitmap->getHeight() : srcHeight,
		RGB(filterR, filterG, filterB)
	);

	SelectObject(bitmapDC, old);
	DeleteDC(bitmapDC);
}

void WindowsRenderer::blitResult(HDC dc, int x, int y, int w, int h)
{
	BitBlt(dc, x, y, w, h, this->target, 0, 0, SRCCOPY);
}


HPEN WindowsRenderer::defaultPen()
{
	this->currentColor = RGB(0, 0, 0);
	this->currentLineWidth = 1;

	return this->createCurrentPen();
}

HBRUSH WindowsRenderer::defaultBrush()
{
	this->currentFillColor = RGB(0, 0, 0);
	return this->createCurrentBrush();
}

HPEN WindowsRenderer::createCurrentPen()
{
	return CreatePen(this->penStyle, this->currentLineWidth, this->currentColor);
}

HBRUSH WindowsRenderer::createCurrentBrush()
{
	return CreateSolidBrush(this->currentFillColor);
}


void WindowsRenderer::switchPen(HPEN pen)
{
	SelectObject(this->target, pen);
	DeleteObject(this->currentPen);
	this->currentPen = pen;
}

void WindowsRenderer::switchBrush(HBRUSH brush)
{
	SelectObject(this->target, brush);
	DeleteObject(this->currentBrush);
	this->currentBrush = brush;
}

Vector2i WindowsRenderer::transform(Vector2 coord)
{
	return Vector2i(
		floor(coord.x * this->coordScale + this->coordOffset.x),
		floor (coord.y * this->coordScale + this->coordOffset.y)
	);
}
