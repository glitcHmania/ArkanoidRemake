#pragma once
#include "Vec2.h"
#include "Graphics.h"
#include "RectF.h"
#include "Ball.h"
class Brick
{
public:
	Brick(Vec2& in_topLeft, float in_width, float in_height, Color in_c);
	Brick(Vec2& in_topLeft, Vec2& bottomRight, Color in_c);
	void Draw(Graphics& gfx);
	RectF GetRect();
	bool BallCollision(Ball& ball);
private:
	bool isDestroyed = false;
	Color c;
	float right;
	float left;
	float top;
	float bottom;
};