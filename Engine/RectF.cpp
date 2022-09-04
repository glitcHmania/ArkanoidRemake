#include "RectF.h"

RectF::RectF(float in_right, float in_left, float in_top, float in_bottom)
	:
	right(in_right),
	left(in_left),
	top(in_top),
	bottom(in_bottom)
{
}

RectF::RectF(Vec2& topLeft, Vec2& bottomRight)
	:
	RectF(bottomRight.x, topLeft.x, topLeft.y, bottomRight.y)
{
}

RectF::RectF(Vec2& topLeft, float width, float height)
	:
	right(topLeft.x + width),
	left(topLeft.x),
	top(topLeft.y),
	bottom(topLeft.y + height)
{
}

RectF RectF::ByCenter(Vec2& center, float halfWidth, float halfHeight)
{
	const Vec2 half(halfWidth, halfHeight);
	return RectF(center - half, halfWidth * 2, halfHeight * 2);
}
