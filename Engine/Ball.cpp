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
		cooldownActive = false;
		pos.x += walls.left - GetRect().left;
		BounceX();
		isColliding = true;
	}
	if (GetRect().right > walls.right)
	{
		cooldownActive = false;
		pos.x += walls.right - GetRect().right;
		BounceX();
		isColliding = true;
	}
	if (GetRect().top < walls.top)
	{
		cooldownActive = false;
		pos.y += walls.top - GetRect().top;
		BounceY();
		isColliding = true;
	}
	if (GetRect().bottom > walls.bottom)
	{
		cooldownActive = false;
		pos.y += walls.bottom - GetRect().bottom;
		BounceY();
		isColliding = true;
	}
	return isColliding;
}

bool Ball::BottomCollision(const RectF& walls)
{
	if (GetRect().bottom > walls.bottom - 0.01f)
	{
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

void Ball::SetVelX(float new_velX)
{
	vel.x = new_velX;
}

void Ball::SetVel(Vec2& new_vel)
{
	vel = new_vel;
}

Vec2 Ball::GetPos()
{
	return pos;
}

void Ball::SetPosX(float new_posX)
{
	pos.x = new_posX;
}

void Ball::SetCooldown(bool boolean)
{
	cooldownActive = boolean;
}

bool Ball::GetCooldownStatus()
{
	return cooldownActive;
}
