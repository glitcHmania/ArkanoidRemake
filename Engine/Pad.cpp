#include "Pad.h"

Pad::Pad(Vec2& in_pos, float in_halfWidth, float in_halfHeight, Color in_mainC, Color in_sideC)
	:
	pos(in_pos),
	halfWidth(in_halfWidth),
	halfHeight(in_halfHeight),
	mainC(in_mainC),
	sideC(in_sideC)
{
}

void Pad::Update(float deltaTime, Keyboard& kbd, RectF& walls)
{
	if (pos.x < walls.right - halfWidth)
	{
		if (kbd.KeyIsPressed(VK_RIGHT))
		{
			pos.x += vel * deltaTime;
		}
	}
	if (pos.x > walls.left + halfWidth)
	{
		if (kbd.KeyIsPressed(VK_LEFT))
		{
			pos.x -= vel * deltaTime;
		}
	}
}

void Pad::Draw(Graphics& gfx)
{
	gfx.DrawRect(GetRect(), sideC);
	gfx.DrawRect(GetMainRect(), mainC);
}

RectF Pad::GetRect()
{
	const Vec2 half(halfWidth, halfHeight);
	return RectF(pos - half, pos + half);
}

RectF Pad::GetMainRect()
{
	return RectF(pos.x + halfWidth - sideSize, pos.x - halfWidth + sideSize, pos.y + halfHeight, pos.y - halfHeight);
}

bool Pad::BallCollision(Ball& ball)
{
	if ( GetRect().isCollidingWith(ball.GetRect()) )
	{
		if (ball.GetVel().y > 0)
		{
			ball.BounceY();
		}
		return true;
	}
	return false;
}

void Pad::WallCollision(RectF& walls)
{
	const RectF rect = GetRect();
	if (rect.left < walls.left)
	{
		pos.x += walls.left - rect.left;
	}
	if (rect.right > walls.right)
	{
		pos.x += walls.right - rect.right;
	}

}
