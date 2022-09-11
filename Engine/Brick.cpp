#include "Brick.h"
#include <assert.h>
#include <cmath>

Brick::Brick(Vec2& in_topLeft, float in_width, float in_height, Color in_c)
	:
	right(in_topLeft.x + in_width),
	left(in_topLeft.x),
	top(in_topLeft.y),
	bottom(in_topLeft.y + in_height),
	c(in_c)
{
}

Brick::Brick(Vec2& in_topLeft, Vec2& in_bottomRight, Color in_c)
	:
	right(in_bottomRight.x),
	left(in_topLeft.x),
	top(in_topLeft.y),
	bottom(in_bottomRight.y),
	c(in_c)
{
}

Brick::Brick(float in_right, float in_left, float in_top, float in_bottom, Color in_c)
	:
	right(in_right),
	left(in_left),
	top(in_top),
	bottom(in_bottom),
	c(in_c)
{
}

void Brick::Draw(Graphics& gfx)
{
	const RectF rect = GetRect();
	if (!isDestroyed)
	{
		gfx.DrawRect(int(rect.left + gapSize), int(rect.top + gapSize), int(rect.right - gapSize), int(rect.bottom - gapSize), c);
	}
}

RectF Brick::GetRect()
{
	return RectF(Vec2(left,top), Vec2(right,bottom));
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
