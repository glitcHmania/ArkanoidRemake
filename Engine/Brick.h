#pragma once
#include "Vec2.h"
#include "Graphics.h"
#include "RectF.h"
#include "Ball.h"
class Brick
{
public:
	Brick() = default;
	Brick(Vec2& in_topLeft, float in_width, float in_height, Color in_c1, Color in_c2);
	Brick(Vec2& in_topLeft, Vec2& bottomRight, Color in_c1, Color in_c2);
	Brick(float in_right, float in_left, float in_top, float in_bottom, Color in_c1, Color in_c2);
	void Draw(Graphics& gfx);
	RectF GetRect();
	RectF GetCenterRect();
	bool CheckBallCollision(Ball& ball);
	void ExecuteBallCollision(Ball& ball);
	Vec2 GetCenter();
private:
	bool isDestroyed = false;
	Color c1, c2;
	float right;
	float left;
	float top;
	float bottom;
	float gapSize = 1.0f;
};