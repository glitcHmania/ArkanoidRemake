#include "Ball.h"

Ball::Ball(Vec2& in_pos, Vec2& in_vel)
	:
	pos(in_pos),
	vel(in_vel)
{
}

void Ball::Draw( Graphics& gfx)
{
	SpriteCodex::DrawBall(pos, gfx);
}

void Ball::Update(float deltaTime)
{
	pos += vel * deltaTime;
}

RectF Ball::GetRect()
{
	return RectF::ByCenter(pos, radius, radius);
}

bool Ball::WallCollision(const RectF& walls)
{
	bool isColliding = false;
	if (GetRect().left < walls.left)
	{
		pos.x += walls.left - GetRect().left;
		BounceX();
		isColliding = true;
	}
	if (GetRect().right > walls.right)
	{
		pos.x += walls.right - GetRect().right;
		BounceX();
		isColliding = true;
	}
	if (GetRect().top < walls.top)
	{
		pos.y += walls.top - GetRect().top;
		BounceY();
		isColliding = true;
	}
	if (GetRect().bottom > walls.bottom)
	{
		pos.y += walls.bottom - GetRect().bottom;
		BounceY();
		isColliding = true;
	}
	return isColliding;
}

bool Ball::BottomCollision(const RectF& walls)
{
	if (GetRect().bottom > walls.bottom - 7.0f)
	{
		pos.y += walls.bottom - GetRect().bottom;
		return true;
	}
	return false;
}

void Ball::BounceX()
{
	vel.x = -vel.x;
}

void Ball::BounceY()
{
	vel.y = -vel.y;
}

Vec2 Ball::GetVel()
{
	return vel;
}

Vec2 Ball::GetPos()
{
	return pos;
}
