#pragma once
#include "Vec2.h"
#include "SpriteCodex.h"
#include "Graphics.h"
#include "RectF.h"
class Ball
{
public:
	Ball(Vec2& in_pos, Vec2& in_vel);
	void Draw( Graphics& gfx);
	void Update(float deltaTime);
	RectF GetRect();
	bool WallCollision(const RectF& walls);
	bool BottomCollision(const RectF& walls);
	void BounceX();
	void BounceY();
	Vec2 GetVel();
	void SetVelX(float new_velX);
	void SetVel(Vec2& new_vel);
	Vec2 GetPos();
	void SetPosX(float new_posX);
	void SetCooldown(bool boolean);
	bool GetCooldownStatus();
private:
	Vec2 pos;
	Vec2 vel;
	float radius = 7.0f;
	bool cooldownActive = false;
};

