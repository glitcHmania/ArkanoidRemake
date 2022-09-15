#include "Brick.h"
#include <assert.h>
#include <cmath>

Brick::Brick(Vec2& in_topLeft, float in_width, float in_height, Color in_c1, Color in_c2)
	:
	right(in_topLeft.x + in_width),
	left(in_topLeft.x),
	top(in_topLeft.y),
	bottom(in_topLeft.y + in_height),
	c1(in_c1),
	c2(in_c2)
{
}

Brick::Brick(Vec2& in_topLeft, Vec2& in_bottomRight, Color in_c1, Color in_c2)
	:
	right(in_bottomRight.x),
	left(in_topLeft.x),
	top(in_topLeft.y),
	bottom(in_bottomRight.y),
	c1(in_c1),
	c2(in_c2)
{
}

Brick::Brick(float in_right, float in_left, float in_top, float in_bottom, Color in_c1, Color in_c2)
	:
	right(in_right),
	left(in_left),
	top(in_top),
	bottom(in_bottom),
	c1(in_c1),
	c2(in_c2)
{
}

void Brick::Draw(Graphics& gfx)
{
	const RectF rect = GetRect();
	if (!isDestroyed)
	{
		gfx.DrawRect(int(rect.left + gapSize), int(rect.top + gapSize), int(rect.right - gapSize), int(rect.bottom - gapSize), c1);
		gfx.DrawRect(GetCenterRect(), c2);
	}
}

RectF Brick::GetRect()
{
	return RectF(Vec2(left,top), Vec2(right,bottom));
}

RectF Brick::GetCenterRect()
{
	RectF rect = GetRect();
	const float gap = 3.0f;
	return RectF(rect.right - gap, rect.left + gap, rect.top + gap, rect.bottom - gap);
}

bool Brick::CheckBallCollision(Ball& ball)
{
	return !isDestroyed && GetRect().isCollidingWith(ball.GetRect());
}

void Brick::ExecuteBallCollision(Ball& ball)
{
	assert(CheckBallCollision(ball));
	if (signbit(ball.GetVel().x) == signbit(ball.GetPos().x - GetCenter().x))
	{
		ball.BounceY();
	}
	else if (ball.GetPos().x < right && ball.GetPos().x > left)
	{
		ball.BounceY();
	}
	else
	{
		ball.BounceX();
	}
	isDestroyed = true;
}

Vec2 Brick::GetCenter()
{
	return GetRect().GetCenter();
}
