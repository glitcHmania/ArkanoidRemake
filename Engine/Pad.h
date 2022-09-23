#pragma once
#include "Vec2.h"
#include "Graphics.h"
#include "RectF.h"
#include "Ball.h"
#include "Keyboard.h"
#include "Mouse.h"

class Pad
{
public:
	Pad(Vec2& in_pos, float in_halfWidth, float in_halfHeight);
	void Update(float deltaTime, Keyboard& kbd, RectF& walls, Mouse& ms);
	void Draw(Graphics& gfx, Vec2& center);
	RectF GetRect();
	RectF GetRightCornerRect();
	RectF GetLeftCornerRect();
	bool BallCollision(Ball& ball, Keyboard& kbd);
	void WallCollision(RectF& walls);
	Vec2 GetCenter();

private:
	Vec2 pos;
	float vel = 550.0f;
	float halfWidth;
	float halfHeight;
	float sideSize = 5.0f;
	const float cornerZoneSize = 4.0f;
	float friction = 0.2f;
};

