#include "Pad.h"

Pad::Pad(Vec2& in_pos, float in_halfWidth, float in_halfHeight)
	:
	pos(in_pos),
	halfWidth(in_halfWidth),
	halfHeight(in_halfHeight)
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

void Pad::Draw(Graphics& gfx, Vec2& center)
{
	int x = int(center.x - halfWidth);
	int y = int(center.y - halfHeight);
	//gfx.DrawRect(GetRightCornerRect(), mainC);
	//gfx.DrawRect(GetLeftCornerRect(), mainC);
	gfx.PutPixel(3 + x, 0 + y, 132, 105, 0);
	gfx.PutPixel(4 + x, 0 + y, 132, 105, 0);
	gfx.PutPixel(5 + x, 0 + y, 132, 105, 0);
	gfx.PutPixel(6 + x, 0 + y, 132, 105, 0);
	gfx.PutPixel(7 + x, 0 + y, 132, 105, 0);
	gfx.PutPixel(8 + x, 0 + y, 85, 85, 85);
	gfx.PutPixel(9 + x, 0 + y, 85, 85, 85);
	gfx.PutPixel(10 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(11 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(12 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(13 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(14 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(15 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(16 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(17 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(18 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(19 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(20 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(21 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(22 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(23 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(24 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(25 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(26 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(27 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(28 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(29 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(30 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(31 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(32 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(33 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(34 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(35 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(36 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(37 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(38 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(39 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(40 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(41 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(42 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(43 + x, 0 + y, 0, 166, 5);
	gfx.PutPixel(44 + x, 0 + y, 85, 85, 85);
	gfx.PutPixel(45 + x, 0 + y, 85, 85, 85);
	gfx.PutPixel(46 + x, 0 + y, 132, 105, 0);
	gfx.PutPixel(47 + x, 0 + y, 132, 105, 0);
	gfx.PutPixel(48 + x, 0 + y, 132, 105, 0);
	gfx.PutPixel(49 + x, 0 + y, 132, 105, 0);
	gfx.PutPixel(50 + x, 0 + y, 132, 105, 0);
	gfx.PutPixel(2 + x, 1 + y, 132, 105, 0);
	gfx.PutPixel(3 + x, 1 + y, 132, 105, 0);
	gfx.PutPixel(4 + x, 1 + y, 132, 105, 0);
	gfx.PutPixel(5 + x, 1 + y, 132, 105, 0);
	gfx.PutPixel(6 + x, 1 + y, 132, 105, 0);
	gfx.PutPixel(7 + x, 1 + y, 132, 105, 0);
	gfx.PutPixel(8 + x, 1 + y, 85, 85, 85);
	gfx.PutPixel(9 + x, 1 + y, 85, 85, 85);
	gfx.PutPixel(10 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(11 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(12 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(13 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(14 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(15 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(16 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(17 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(18 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(19 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(20 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(21 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(22 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(23 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(24 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(25 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(26 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(27 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(28 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(29 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(30 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(31 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(32 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(33 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(34 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(35 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(36 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(37 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(38 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(39 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(40 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(41 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(42 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(43 + x, 1 + y, 0, 166, 5);
	gfx.PutPixel(44 + x, 1 + y, 85, 85, 85);
	gfx.PutPixel(45 + x, 1 + y, 85, 85, 85);
	gfx.PutPixel(46 + x, 1 + y, 132, 105, 0);
	gfx.PutPixel(47 + x, 1 + y, 132, 105, 0);
	gfx.PutPixel(48 + x, 1 + y, 132, 105, 0);
	gfx.PutPixel(49 + x, 1 + y, 132, 105, 0);
	gfx.PutPixel(50 + x, 1 + y, 132, 105, 0);
	gfx.PutPixel(51 + x, 1 + y, 132, 105, 0);
	gfx.PutPixel(1 + x, 2 + y, 132, 105, 0);
	gfx.PutPixel(2 + x, 2 + y, 132, 105, 0);
	gfx.PutPixel(3 + x, 2 + y, 132, 105, 0);
	gfx.PutPixel(4 + x, 2 + y, 132, 105, 0);
	gfx.PutPixel(5 + x, 2 + y, 132, 105, 0);
	gfx.PutPixel(6 + x, 2 + y, 132, 105, 0);
	gfx.PutPixel(7 + x, 2 + y, 132, 105, 0);
	gfx.PutPixel(8 + x, 2 + y, 85, 85, 85);
	gfx.PutPixel(9 + x, 2 + y, 85, 85, 85);
	gfx.PutPixel(10 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(11 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(12 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(13 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(14 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(15 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(16 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(17 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(18 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(19 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(20 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(21 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(22 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(23 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(24 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(25 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(26 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(27 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(28 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(29 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(30 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(31 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(32 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(33 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(34 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(35 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(36 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(37 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(38 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(39 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(40 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(41 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(42 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(43 + x, 2 + y, 0, 166, 5);
	gfx.PutPixel(44 + x, 2 + y, 85, 85, 85);
	gfx.PutPixel(45 + x, 2 + y, 85, 85, 85);
	gfx.PutPixel(46 + x, 2 + y, 132, 105, 0);
	gfx.PutPixel(47 + x, 2 + y, 132, 105, 0);
	gfx.PutPixel(48 + x, 2 + y, 132, 105, 0);
	gfx.PutPixel(49 + x, 2 + y, 132, 105, 0);
	gfx.PutPixel(50 + x, 2 + y, 132, 105, 0);
	gfx.PutPixel(51 + x, 2 + y, 132, 105, 0);
	gfx.PutPixel(52 + x, 2 + y, 132, 105, 0);
	gfx.PutPixel(0 + x, 3 + y, 132, 105, 0);
	gfx.PutPixel(1 + x, 3 + y, 132, 105, 0);
	gfx.PutPixel(2 + x, 3 + y, 132, 105, 0);
	gfx.PutPixel(3 + x, 3 + y, 255, 203, 0);
	gfx.PutPixel(4 + x, 3 + y, 255, 203, 0);
	gfx.PutPixel(5 + x, 3 + y, 255, 203, 0);
	gfx.PutPixel(6 + x, 3 + y, 255, 203, 0);
	gfx.PutPixel(7 + x, 3 + y, 255, 203, 0);
	gfx.PutPixel(8 + x, 3 + y, 195, 195, 195);
	gfx.PutPixel(9 + x, 3 + y, 195, 195, 195);
	gfx.PutPixel(10 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(11 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(12 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(13 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(14 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(15 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(16 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(17 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(18 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(19 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(20 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(21 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(22 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(23 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(24 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(25 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(26 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(27 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(28 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(29 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(30 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(31 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(32 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(33 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(34 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(35 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(36 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(37 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(38 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(39 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(40 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(41 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(42 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(43 + x, 3 + y, 0, 255, 7);
	gfx.PutPixel(44 + x, 3 + y, 195, 195, 195);
	gfx.PutPixel(45 + x, 3 + y, 195, 195, 195);
	gfx.PutPixel(46 + x, 3 + y, 255, 203, 0);
	gfx.PutPixel(47 + x, 3 + y, 255, 203, 0);
	gfx.PutPixel(48 + x, 3 + y, 255, 203, 0);
	gfx.PutPixel(49 + x, 3 + y, 255, 203, 0);
	gfx.PutPixel(50 + x, 3 + y, 255, 203, 0);
	gfx.PutPixel(51 + x, 3 + y, 132, 105, 0);
	gfx.PutPixel(52 + x, 3 + y, 132, 105, 0);
	gfx.PutPixel(53 + x, 3 + y, 132, 105, 0);
	gfx.PutPixel(0 + x, 4 + y, 132, 105, 0);
	gfx.PutPixel(1 + x, 4 + y, 132, 105, 0);
	gfx.PutPixel(2 + x, 4 + y, 255, 203, 0);
	gfx.PutPixel(3 + x, 4 + y, 255, 203, 0);
	gfx.PutPixel(4 + x, 4 + y, 255, 203, 0);
	gfx.PutPixel(5 + x, 4 + y, 255, 203, 0);
	gfx.PutPixel(6 + x, 4 + y, 255, 203, 0);
	gfx.PutPixel(7 + x, 4 + y, 255, 203, 0);
	gfx.PutPixel(8 + x, 4 + y, 195, 195, 195);
	gfx.PutPixel(9 + x, 4 + y, 195, 195, 195);
	gfx.PutPixel(10 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(11 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(12 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(13 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(14 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(15 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(16 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(17 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(18 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(19 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(20 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(21 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(22 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(23 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(24 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(25 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(26 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(27 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(28 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(29 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(30 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(31 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(32 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(33 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(34 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(35 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(36 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(37 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(38 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(39 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(40 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(41 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(42 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(43 + x, 4 + y, 0, 255, 7);
	gfx.PutPixel(44 + x, 4 + y, 195, 195, 195);
	gfx.PutPixel(45 + x, 4 + y, 195, 195, 195);
	gfx.PutPixel(46 + x, 4 + y, 255, 203, 0);
	gfx.PutPixel(47 + x, 4 + y, 255, 203, 0);
	gfx.PutPixel(48 + x, 4 + y, 255, 203, 0);
	gfx.PutPixel(49 + x, 4 + y, 255, 203, 0);
	gfx.PutPixel(50 + x, 4 + y, 255, 203, 0);
	gfx.PutPixel(51 + x, 4 + y, 255, 203, 0);
	gfx.PutPixel(52 + x, 4 + y, 132, 105, 0);
	gfx.PutPixel(53 + x, 4 + y, 132, 105, 0);
	gfx.PutPixel(0 + x, 5 + y, 132, 105, 0);
	gfx.PutPixel(1 + x, 5 + y, 132, 105, 0);
	gfx.PutPixel(2 + x, 5 + y, 255, 203, 0);
	gfx.PutPixel(3 + x, 5 + y, 255, 203, 0);
	gfx.PutPixel(4 + x, 5 + y, 255, 203, 0);
	gfx.PutPixel(5 + x, 5 + y, 255, 203, 0);
	gfx.PutPixel(6 + x, 5 + y, 255, 203, 0);
	gfx.PutPixel(7 + x, 5 + y, 255, 203, 0);
	gfx.PutPixel(8 + x, 5 + y, 195, 195, 195);
	gfx.PutPixel(9 + x, 5 + y, 195, 195, 195);
	gfx.PutPixel(10 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(11 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(12 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(13 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(14 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(15 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(16 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(17 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(18 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(19 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(20 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(21 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(22 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(23 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(24 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(25 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(26 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(27 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(28 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(29 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(30 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(31 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(32 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(33 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(34 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(35 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(36 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(37 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(38 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(39 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(40 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(41 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(42 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(43 + x, 5 + y, 0, 255, 7);
	gfx.PutPixel(44 + x, 5 + y, 195, 195, 195);
	gfx.PutPixel(45 + x, 5 + y, 195, 195, 195);
	gfx.PutPixel(46 + x, 5 + y, 255, 203, 0);
	gfx.PutPixel(47 + x, 5 + y, 255, 203, 0);
	gfx.PutPixel(48 + x, 5 + y, 255, 203, 0);
	gfx.PutPixel(49 + x, 5 + y, 255, 203, 0);
	gfx.PutPixel(50 + x, 5 + y, 255, 203, 0);
	gfx.PutPixel(51 + x, 5 + y, 255, 203, 0);
	gfx.PutPixel(52 + x, 5 + y, 132, 105, 0);
	gfx.PutPixel(53 + x, 5 + y, 132, 105, 0);
	gfx.PutPixel(0 + x, 6 + y, 132, 105, 0);
	gfx.PutPixel(1 + x, 6 + y, 132, 105, 0);
	gfx.PutPixel(2 + x, 6 + y, 255, 203, 0);
	gfx.PutPixel(3 + x, 6 + y, 255, 203, 0);
	gfx.PutPixel(4 + x, 6 + y, 255, 203, 0);
	gfx.PutPixel(5 + x, 6 + y, 255, 203, 0);
	gfx.PutPixel(6 + x, 6 + y, 255, 203, 0);
	gfx.PutPixel(7 + x, 6 + y, 255, 203, 0);
	gfx.PutPixel(8 + x, 6 + y, 195, 195, 195);
	gfx.PutPixel(9 + x, 6 + y, 195, 195, 195);
	gfx.PutPixel(10 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(11 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(12 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(13 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(14 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(15 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(16 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(17 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(18 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(19 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(20 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(21 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(22 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(23 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(24 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(25 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(26 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(27 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(28 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(29 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(30 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(31 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(32 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(33 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(34 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(35 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(36 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(37 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(38 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(39 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(40 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(41 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(42 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(43 + x, 6 + y, 0, 255, 7);
	gfx.PutPixel(44 + x, 6 + y, 195, 195, 195);
	gfx.PutPixel(45 + x, 6 + y, 195, 195, 195);
	gfx.PutPixel(46 + x, 6 + y, 255, 203, 0);
	gfx.PutPixel(47 + x, 6 + y, 255, 203, 0);
	gfx.PutPixel(48 + x, 6 + y, 255, 203, 0);
	gfx.PutPixel(49 + x, 6 + y, 255, 203, 0);
	gfx.PutPixel(50 + x, 6 + y, 255, 203, 0);
	gfx.PutPixel(51 + x, 6 + y, 255, 203, 0);
	gfx.PutPixel(52 + x, 6 + y, 132, 105, 0);
	gfx.PutPixel(53 + x, 6 + y, 132, 105, 0);
	gfx.PutPixel(0 + x, 7 + y, 132, 105, 0);
	gfx.PutPixel(1 + x, 7 + y, 132, 105, 0);
	gfx.PutPixel(2 + x, 7 + y, 255, 203, 0);
	gfx.PutPixel(3 + x, 7 + y, 255, 203, 0);
	gfx.PutPixel(4 + x, 7 + y, 255, 203, 0);
	gfx.PutPixel(5 + x, 7 + y, 255, 203, 0);
	gfx.PutPixel(6 + x, 7 + y, 255, 203, 0);
	gfx.PutPixel(7 + x, 7 + y, 255, 203, 0);
	gfx.PutPixel(8 + x, 7 + y, 195, 195, 195);
	gfx.PutPixel(9 + x, 7 + y, 195, 195, 195);
	gfx.PutPixel(10 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(11 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(12 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(13 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(14 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(15 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(16 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(17 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(18 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(19 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(20 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(21 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(22 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(23 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(24 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(25 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(26 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(27 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(28 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(29 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(30 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(31 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(32 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(33 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(34 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(35 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(36 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(37 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(38 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(39 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(40 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(41 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(42 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(43 + x, 7 + y, 0, 255, 7);
	gfx.PutPixel(44 + x, 7 + y, 195, 195, 195);
	gfx.PutPixel(45 + x, 7 + y, 195, 195, 195);
	gfx.PutPixel(46 + x, 7 + y, 255, 203, 0);
	gfx.PutPixel(47 + x, 7 + y, 255, 203, 0);
	gfx.PutPixel(48 + x, 7 + y, 255, 203, 0);
	gfx.PutPixel(49 + x, 7 + y, 255, 203, 0);
	gfx.PutPixel(50 + x, 7 + y, 255, 203, 0);
	gfx.PutPixel(51 + x, 7 + y, 255, 203, 0);
	gfx.PutPixel(52 + x, 7 + y, 132, 105, 0);
	gfx.PutPixel(53 + x, 7 + y, 132, 105, 0);
	gfx.PutPixel(0 + x, 8 + y, 132, 105, 0);
	gfx.PutPixel(1 + x, 8 + y, 132, 105, 0);
	gfx.PutPixel(2 + x, 8 + y, 255, 203, 0);
	gfx.PutPixel(3 + x, 8 + y, 255, 203, 0);
	gfx.PutPixel(4 + x, 8 + y, 255, 203, 0);
	gfx.PutPixel(5 + x, 8 + y, 255, 203, 0);
	gfx.PutPixel(6 + x, 8 + y, 255, 203, 0);
	gfx.PutPixel(7 + x, 8 + y, 255, 203, 0);
	gfx.PutPixel(8 + x, 8 + y, 195, 195, 195);
	gfx.PutPixel(9 + x, 8 + y, 195, 195, 195);
	gfx.PutPixel(10 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(11 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(12 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(13 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(14 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(15 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(16 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(17 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(18 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(19 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(20 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(21 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(22 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(23 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(24 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(25 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(26 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(27 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(28 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(29 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(30 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(31 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(32 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(33 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(34 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(35 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(36 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(37 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(38 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(39 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(40 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(41 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(42 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(43 + x, 8 + y, 0, 255, 7);
	gfx.PutPixel(44 + x, 8 + y, 195, 195, 195);
	gfx.PutPixel(45 + x, 8 + y, 195, 195, 195);
	gfx.PutPixel(46 + x, 8 + y, 255, 203, 0);
	gfx.PutPixel(47 + x, 8 + y, 255, 203, 0);
	gfx.PutPixel(48 + x, 8 + y, 255, 203, 0);
	gfx.PutPixel(49 + x, 8 + y, 255, 203, 0);
	gfx.PutPixel(50 + x, 8 + y, 255, 203, 0);
	gfx.PutPixel(51 + x, 8 + y, 255, 203, 0);
	gfx.PutPixel(52 + x, 8 + y, 132, 105, 0);
	gfx.PutPixel(53 + x, 8 + y, 132, 105, 0);
	gfx.PutPixel(0 + x, 9 + y, 132, 105, 0);
	gfx.PutPixel(1 + x, 9 + y, 132, 105, 0);
	gfx.PutPixel(2 + x, 9 + y, 255, 203, 0);
	gfx.PutPixel(3 + x, 9 + y, 255, 203, 0);
	gfx.PutPixel(4 + x, 9 + y, 255, 203, 0);
	gfx.PutPixel(5 + x, 9 + y, 255, 203, 0);
	gfx.PutPixel(6 + x, 9 + y, 255, 203, 0);
	gfx.PutPixel(7 + x, 9 + y, 255, 203, 0);
	gfx.PutPixel(8 + x, 9 + y, 195, 195, 195);
	gfx.PutPixel(9 + x, 9 + y, 195, 195, 195);
	gfx.PutPixel(10 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(11 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(12 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(13 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(14 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(15 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(16 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(17 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(18 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(19 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(20 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(21 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(22 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(23 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(24 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(25 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(26 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(27 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(28 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(29 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(30 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(31 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(32 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(33 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(34 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(35 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(36 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(37 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(38 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(39 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(40 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(41 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(42 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(43 + x, 9 + y, 0, 255, 7);
	gfx.PutPixel(44 + x, 9 + y, 195, 195, 195);
	gfx.PutPixel(45 + x, 9 + y, 195, 195, 195);
	gfx.PutPixel(46 + x, 9 + y, 255, 203, 0);
	gfx.PutPixel(47 + x, 9 + y, 255, 203, 0);
	gfx.PutPixel(48 + x, 9 + y, 255, 203, 0);
	gfx.PutPixel(49 + x, 9 + y, 255, 203, 0);
	gfx.PutPixel(50 + x, 9 + y, 255, 203, 0);
	gfx.PutPixel(51 + x, 9 + y, 255, 203, 0);
	gfx.PutPixel(52 + x, 9 + y, 132, 105, 0);
	gfx.PutPixel(53 + x, 9 + y, 132, 105, 0);
	gfx.PutPixel(0 + x, 10 + y, 132, 105, 0);
	gfx.PutPixel(1 + x, 10 + y, 132, 105, 0);
	gfx.PutPixel(2 + x, 10 + y, 132, 105, 0);
	gfx.PutPixel(3 + x, 10 + y, 255, 203, 0);
	gfx.PutPixel(4 + x, 10 + y, 255, 203, 0);
	gfx.PutPixel(5 + x, 10 + y, 255, 203, 0);
	gfx.PutPixel(6 + x, 10 + y, 255, 203, 0);
	gfx.PutPixel(7 + x, 10 + y, 255, 203, 0);
	gfx.PutPixel(8 + x, 10 + y, 195, 195, 195);
	gfx.PutPixel(9 + x, 10 + y, 195, 195, 195);
	gfx.PutPixel(10 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(11 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(12 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(13 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(14 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(15 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(16 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(17 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(18 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(19 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(20 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(21 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(22 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(23 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(24 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(25 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(26 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(27 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(28 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(29 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(30 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(31 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(32 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(33 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(34 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(35 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(36 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(37 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(38 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(39 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(40 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(41 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(42 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(43 + x, 10 + y, 0, 255, 7);
	gfx.PutPixel(44 + x, 10 + y, 195, 195, 195);
	gfx.PutPixel(45 + x, 10 + y, 195, 195, 195);
	gfx.PutPixel(46 + x, 10 + y, 255, 203, 0);
	gfx.PutPixel(47 + x, 10 + y, 255, 203, 0);
	gfx.PutPixel(48 + x, 10 + y, 255, 203, 0);
	gfx.PutPixel(49 + x, 10 + y, 255, 203, 0);
	gfx.PutPixel(50 + x, 10 + y, 255, 203, 0);
	gfx.PutPixel(51 + x, 10 + y, 132, 105, 0);
	gfx.PutPixel(52 + x, 10 + y, 132, 105, 0);
	gfx.PutPixel(53 + x, 10 + y, 132, 105, 0);
	gfx.PutPixel(1 + x, 11 + y, 132, 105, 0);
	gfx.PutPixel(2 + x, 11 + y, 132, 105, 0);
	gfx.PutPixel(3 + x, 11 + y, 132, 105, 0);
	gfx.PutPixel(4 + x, 11 + y, 132, 105, 0);
	gfx.PutPixel(5 + x, 11 + y, 132, 105, 0);
	gfx.PutPixel(6 + x, 11 + y, 132, 105, 0);
	gfx.PutPixel(7 + x, 11 + y, 132, 105, 0);
	gfx.PutPixel(8 + x, 11 + y, 85, 85, 85);
	gfx.PutPixel(9 + x, 11 + y, 85, 85, 85);
	gfx.PutPixel(10 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(11 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(12 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(13 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(14 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(15 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(16 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(17 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(18 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(19 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(20 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(21 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(22 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(23 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(24 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(25 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(26 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(27 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(28 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(29 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(30 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(31 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(32 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(33 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(34 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(35 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(36 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(37 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(38 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(39 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(40 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(41 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(42 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(43 + x, 11 + y, 0, 166, 5);
	gfx.PutPixel(44 + x, 11 + y, 85, 85, 85);
	gfx.PutPixel(45 + x, 11 + y, 85, 85, 85);
	gfx.PutPixel(46 + x, 11 + y, 132, 105, 0);
	gfx.PutPixel(47 + x, 11 + y, 132, 105, 0);
	gfx.PutPixel(48 + x, 11 + y, 132, 105, 0);
	gfx.PutPixel(49 + x, 11 + y, 132, 105, 0);
	gfx.PutPixel(50 + x, 11 + y, 132, 105, 0);
	gfx.PutPixel(51 + x, 11 + y, 132, 105, 0);
	gfx.PutPixel(52 + x, 11 + y, 132, 105, 0);
	gfx.PutPixel(2 + x, 12 + y, 132, 105, 0);
	gfx.PutPixel(3 + x, 12 + y, 132, 105, 0);
	gfx.PutPixel(4 + x, 12 + y, 132, 105, 0);
	gfx.PutPixel(5 + x, 12 + y, 132, 105, 0);
	gfx.PutPixel(6 + x, 12 + y, 132, 105, 0);
	gfx.PutPixel(7 + x, 12 + y, 132, 105, 0);
	gfx.PutPixel(8 + x, 12 + y, 85, 85, 85);
	gfx.PutPixel(9 + x, 12 + y, 85, 85, 85);
	gfx.PutPixel(10 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(11 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(12 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(13 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(14 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(15 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(16 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(17 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(18 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(19 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(20 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(21 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(22 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(23 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(24 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(25 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(26 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(27 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(28 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(29 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(30 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(31 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(32 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(33 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(34 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(35 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(36 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(37 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(38 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(39 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(40 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(41 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(42 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(43 + x, 12 + y, 0, 166, 5);
	gfx.PutPixel(44 + x, 12 + y, 85, 85, 85);
	gfx.PutPixel(45 + x, 12 + y, 85, 85, 85);
	gfx.PutPixel(46 + x, 12 + y, 132, 105, 0);
	gfx.PutPixel(47 + x, 12 + y, 132, 105, 0);
	gfx.PutPixel(48 + x, 12 + y, 132, 105, 0);
	gfx.PutPixel(49 + x, 12 + y, 132, 105, 0);
	gfx.PutPixel(50 + x, 12 + y, 132, 105, 0);
	gfx.PutPixel(51 + x, 12 + y, 132, 105, 0);
	gfx.PutPixel(3 + x, 13 + y, 132, 105, 0);
	gfx.PutPixel(4 + x, 13 + y, 132, 105, 0);
	gfx.PutPixel(5 + x, 13 + y, 132, 105, 0);
	gfx.PutPixel(6 + x, 13 + y, 132, 105, 0);
	gfx.PutPixel(7 + x, 13 + y, 132, 105, 0);
	gfx.PutPixel(8 + x, 13 + y, 85, 85, 85);
	gfx.PutPixel(9 + x, 13 + y, 85, 85, 85);
	gfx.PutPixel(10 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(11 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(12 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(13 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(14 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(15 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(16 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(17 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(18 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(19 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(20 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(21 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(22 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(23 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(24 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(25 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(26 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(27 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(28 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(29 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(30 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(31 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(32 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(33 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(34 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(35 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(36 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(37 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(38 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(39 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(40 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(41 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(42 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(43 + x, 13 + y, 0, 166, 5);
	gfx.PutPixel(44 + x, 13 + y, 85, 85, 85);
	gfx.PutPixel(45 + x, 13 + y, 85, 85, 85);
	gfx.PutPixel(46 + x, 13 + y, 132, 105, 0);
	gfx.PutPixel(47 + x, 13 + y, 132, 105, 0);
	gfx.PutPixel(48 + x, 13 + y, 132, 105, 0);
	gfx.PutPixel(49 + x, 13 + y, 132, 105, 0);
	gfx.PutPixel(50 + x, 13 + y, 132, 105, 0);


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
