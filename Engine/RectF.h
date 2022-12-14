#pragma once
#include "Vec2.h"
class RectF
{
public:
	RectF() = default;
	RectF(float in_right, float in_left, float in_top, float in_bottom);
	RectF(Vec2& topLeft, Vec2& bottomRight);
	RectF(Vec2& topLeft, float width, float height);
	static RectF ByCenter(Vec2& center, float halfWidth, float halfHeight);
	bool isCollidingWith(const RectF& otherRect) const;
	Vec2 GetCenter();
	float right;
	float left;
	float top;
	float bottom;
	float widht = right - left;
	float height = top - bottom;
private:
};

