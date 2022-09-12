/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "RectF.h"
#include "Ball.h"
#include "FrameTimer.h"
#include "Brick.h"
#include "Pad.h"
#include "Vec2.h"

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel(float deltaTime);
	/********************************/
	/*  User Functions              */
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	bool gameStarted = false;
	bool gameOver = false;
	RectF walls;
	Ball ball;
	FrameTimer ft;
	const Vec2 brickStartPos = Vec2(10.0f, 10.0f);
	static constexpr int bricksRows = 4;
	static constexpr int bricksColumns = 15;
	static constexpr int nBricks = bricksRows * bricksColumns;
	const float brickWidth = 52.0f;
	const float brickHeight = 20.0f;
	Brick bricks[nBricks];
	Pad pad;
	float ballStartVelVertical = -300.0f;
	float ballStartPosVertical = 539.0f;
	/********************************/
};