#include "Brick.h"

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

void Brick::Draw(Graphics& gfx)
{
	const RectF rect = GetRect();
	if (!isDestroyed)
	{
		gfx.DrawRect(rect, c);
	}
}

RectF Brick::GetRect()
{
	return RectF(Vec2(left,top), Vec2(right,bottom));
}

bool Brick::BallCollision(Ball& ball)
{
	if (!isDestroyed && GetRect().isCollidingWith(ball.GetRect()))
	{
		ball.BounceY();
		isDestroyed = true;
		return true;
	}
	return false;
}
