#pragma once
#include "Vec2.h"
#include "Graphics.h"
#include "RectF.h"
#include "Ball.h"
#include "Keyboard.h"

class Pad
{
public:
	Pad(Vec2& in_pos, float in_halfWidth, float in_halfHeight, Color in_mainC, Color in_sideC);
	void Update(float deltaTime, Keyboard& kbd, RectF& walls);
	void Draw(Graphics& gfx);
	RectF GetRect();
	RectF GetRightCornerRect();
	RectF GetLeftCornerRect();
	RectF GetMainRect();
	bool BallCollision(Ball& ball, Keyboard& kbd);
	bool BallCornerCollision(Ball& ball);
	void WallCollision(RectF& walls);
	Vec2 GetCenter();

private:
	Vec2 pos;
	float vel = 550.0f;
	float halfWidth;
	float halfHeight;
	float sideSize = 5.0f;
	Color mainC;
	Color sideC;
	const float cornerZoneSize = 4.0f;
	float friction = 0.5f;
};

