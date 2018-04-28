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

	if (this->target) {
		DeleteDC(this->target);
	}
}

HDC Renderer::releaseDC()
{
	HDC dc = this->target;
	this->target = nullptr;
	return dc;
}

void Renderer::scale(double scale)
{
	this->coordScale *= scale;
}

void Renderer::offset(Vector2 offset)
{
	this->coordOffset += offset * this->coordScale;
}

Vector2 Renderer::getOffset()
{
	return this->coordOffset;
}

void Renderer::setColor(int r, int g, int b)
{
	this->currentColor = RGB(r, g, b);
	this->switchPen(this->createCurrentPen());
}

void Renderer::setColor(Color color)
{
	color.clampColors();
	this->setColor(color.r * 255, color.g * 255, color.b * 255);
}

void Renderer::setFillColor(int r, int g, int b)
{
	this->currentFillColor = RGB(r, g, b);
	this->switchBrush(this->createCurrentBrush());
}

void Renderer::setFillColor(Color color)
{
	color.clampColors();
	this->setFillColor(color.r * 255, color.g * 255, color.b * 255);
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
	this->textColor = RGB(r, g, b);
	SetTextColor(this->target, this->textColor);
}

void Renderer::setTextColor(Color color)
{
	color.clampColors();
	this->setTextColor(color.r * 255, color.g * 255, color.b * 255);
}

void Renderer::setTextBackgroundColor(int r, int g, int b)
{
	SetBkColor(this->target, RGB(r, g, b));
}

void Renderer::setTextBackgroundColor(Color color)
{
	color.clampColors();
	this->setTextBackgroundColor(color.r * 255, color.g * 255, color.b * 255);
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

void Renderer::fillRect(BoundingBox box)
{
	this->fillRect(box.left, box.top, box.right - box.left, box.bottom - box.top);
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

void Renderer::drawRect(BoundingBox box)
{
	this->drawRect(box.left, box.top, box.right - box.left, box.bottom - box.top);
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
	DrawText(this->target, text.c_str(), text.size(), &rect, DT_VCENTER | DT_SINGLELINE | DT_LEFT | DT_NOCLIP);
}

void Renderer::drawTextLeftAligned(std::string text, BoundingBox box)
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

void Renderer::drawTextCentered(std::string text, RECT rect)
{
	DrawText(this->target, text.c_str(), text.size(), &rect, DT_VCENTER | DT_CENTER | DT_SINGLELINE | DT_NOCLIP);
}

void Renderer::drawTextCentered(std::string text, BoundingBox box)
{
	this->drawTextCentered(text, box.left, box.right, box.top, box.bottom);
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

void Renderer::drawTextRightAligned(std::string text, RECT rect)
{
	DrawText(this->target, text.c_str(), text.size(), &rect, DT_VCENTER | DT_SINGLELINE | DT_RIGHT | DT_NOCLIP);
}

void Renderer::drawTextRightAligned(std::string text, BoundingBox box)
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

void Renderer::drawBitmap(Bitmap& bitmap, int x, int y, int width, int height, int srcX, int srcY)
{
	HDC bitmapDC = CreateCompatibleDC(this->target);
	HBITMAP old = bitmap.selectInto(bitmapDC);

	BitBlt(
		this->target,
		x, y,
		width < 0 ? bitmap.getWidth() : width, height < 0 ? bitmap.getHeight() : height,
		bitmapDC,
		srcX, srcY,
		SRCCOPY
	);

	SelectObject(bitmapDC, old);
	DeleteDC(bitmapDC);
}

void Renderer::drawBitmapTransparent(Bitmap & bitmap, int filterR, int filterG, int filterB, int x, int y, int width, int height, int srcX, int srcY, int srcWidth, int srcHeight)
{
	HDC bitmapDC = CreateCompatibleDC(this->target);
	HBITMAP old = bitmap.selectInto(bitmapDC);

	TransparentBlt(
		this->target,
		x, y, 
		width < 0 ? bitmap.getWidth() : width, height < 0 ? bitmap.getHeight() : height,
		bitmapDC, 
		srcX, srcY, 
		srcWidth < 0 ? bitmap.getWidth() : srcWidth, srcHeight < 0 ? bitmap.getHeight() : srcHeight,
		RGB(filterR, filterG, filterB)
	);

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
		floor(coord.x * this->coordScale + this->coordOffset.x),
		floor (coord.y * this->coordScale + this->coordOffset.y)
	);
}
