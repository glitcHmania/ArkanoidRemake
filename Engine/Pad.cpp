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
	//gfx.DrawRect(GetRightCornerRect(), mainC);
	//gfx.DrawRect(GetLeftCornerRect(), mainC);
}

RectF Pad::GetRect()
{
	const Vec2 half(halfWidth, halfHeight);
	return RectF(pos - half, pos + half);
}

RectF Pad::GetRightCornerRect()
{
	return RectF(pos.x + halfWidth + cornerZoneSize, pos.x + halfWidth - cornerZoneSize, pos.y - halfHeight + cornerZoneSize, pos.y - halfHeight - cornerZoneSize);
}

RectF Pad::GetLeftCornerRect()
{
	return RectF(pos.x - halfWidth + cornerZoneSize, pos.x - halfWidth - cornerZoneSize, pos.y - halfHeight + cornerZoneSize, pos.y - halfHeight - cornerZoneSize);
}

RectF Pad::GetMainRect()
{
	return RectF(pos.x + halfWidth - sideSize, pos.x - halfWidth + sideSize, pos.y + halfHeight, pos.y - halfHeight);
}

bool Pad::BallCollision(Ball& ball, Keyboard& kbd)
{
	if ( GetRect().isCollidingWith(ball.GetRect()))
	{
		if (ball.GetVel().y > 0)
		{
			if (kbd.KeyIsPressed(VK_RIGHT))
			{
				ball.BounceY();
				float newVelX = ball.GetVel().x + (friction * vel);
				Vec2 newVel = Vec2(newVelX, ball.GetVel().y).Normalize() * 550.0f; // Change this multiplier if you change the ball velocity
				ball.SetVel(newVel);
			}
			else if (kbd.KeyIsPressed(VK_LEFT))
			{
				ball.BounceY();
				float newVelX = ball.GetVel().x - (friction * vel);
				Vec2 newVel = Vec2(newVelX, ball.GetVel().y).Normalize() * 550.0f; // Change this multiplier if you change the ball velocity
				ball.SetVel(newVel);
			}
			else
			{
				ball.BounceY();
			}
			return true;
		}
	}
	return false;
}

bool Pad::BallCornerCollision(Ball& ball)
{
	// TOP RIGHT CORNER
	if (GetRightCornerRect().isCollidingWith(ball.GetRect()))
	{
		if(ball.GetVel().x < 0)
		{
			ball.BounceY();
			ball.BounceX();
		}
		return true;
	}
	// TOP LEFT CORNER
	else if (GetLeftCornerRect().isCollidingWith(ball.GetRect()))
	{
		if (ball.GetVel().x > 0)
		{
			ball.BounceY();
			ball.BounceX();
		}
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

Vec2 Pad::GetCenter()
{
	return GetRect().GetCenter();
}
