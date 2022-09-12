/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
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
#include "MainWindow.h"
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	walls(gfx.ScreenWidth, 0.0f, 0.0f, gfx.ScreenHeight),
	ball(Vec2(400.0f, 539.0f), Vec2(-300.0f, -300.0f)),
	pad(Vec2(400.0f, 550.0f), 20, 4, Colors::Green, Colors::Cyan)
{
	Color colorList[4] = { Colors::Red, Colors::Blue, Colors::Green, Colors::Yellow };
	for (int y = 0; y < bricksRows ; ++y)
	{
		for (int x = 0; x < bricksColumns; ++x)
		{
			bricks[x + y * bricksColumns] = Brick(Vec2((x * brickWidth) + brickStartPos.x, (y * brickHeight) + brickStartPos.y), brickWidth, brickHeight, colorList[y % 4]);
		}
	}
}

void Game::Go()
{
	gfx.BeginFrame();

	// deltaTime calculation
	float elapsedTime = ft.Mark();
	while (elapsedTime > 0.0f)
	{
		float deltaTime = std::min(0.0025f, elapsedTime);
		UpdateModel(deltaTime);
		elapsedTime -= deltaTime;
	}
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel(float deltaTime)
{
	// Checking if the "ENTER" is pressed to start the game
	if (!gameStarted && wnd.kbd.KeyIsPressed(VK_RETURN))
	{
		gameStarted = true;
	}

	// Checking if the ball is colliding with bottom wall to determine the game over status
	//if (ball.BottomCollision(walls))
	//{
	//	gameOver = true;
	//}

	// Checking if the game is active
	if (gameStarted && !gameOver)
	{
		// Making the ball move
		ball.Update(deltaTime);

		// Calculating the ball/outer-walls collision
		ball.WallCollision(walls);

		// Making the ball hit the closest colliding brick 
		int currentColIndex;
		float currentColDistSq;
		bool hasCollided = false;
		for (int i = 0; i < nBricks; i++)
		{
			if (bricks[i].CheckBallCollision(ball))
			{
				const float newColDistSq = (ball.GetPos() - bricks[i].GetCenter()).GetLengthSq();
				if (hasCollided)
				{
					if (newColDistSq < currentColDistSq)
					{
						currentColDistSq = newColDistSq;
						currentColIndex = i;
					}
				}
				else
				{
					currentColDistSq = newColDistSq;
					currentColIndex = i;
					hasCollided = true;
				}
			}
		}

		// Checking if the ball/brick collision is happened
		if (hasCollided)
		{
			bricks[currentColIndex].ExecuteBallCollision(ball);
		}

		// Calculating the pad-corner-hitbox/ball collision for diagonal bounce situation
		//pad.BallCornerCollision(ball);

		// Calculating the simple pad/ball collision
		pad.BallCollision(ball);
	}

	// Checking if the title screen is active
	if(!gameStarted && !gameOver)
	{
		// Making the ball stick to the pad in the title screen
		ball.SetPosX(pad.GetCenter().x);

		// Making the ball bounce to the center of the screen
		if (pad.GetCenter().x < gfx.ScreenWidth / 2)
		{
			ball.SetVelX(300.0f);
		}
		else
		{
			ball.SetVelX(-300.0f);
		}
	}

	// Checking if the game is over
	if (!gameOver)
	{
		// Calculating the pad/wall collision
		pad.WallCollision(walls);

		// Making the pad move
		pad.Update(deltaTime, wnd.kbd, walls);
	}
}

void Game::ComposeFrame()
{
	// Drawing the game objects
	for (Brick& brick : bricks)
	{
		brick.Draw(gfx);
	}
	ball.Draw(gfx);
	pad.Draw(gfx);

	// Drawing the title and game over screen
	if (gameOver)
	{
		gfx.DrawGameOver(gfx.ScreenWidth / 2 - 25, gfx.ScreenHeight / 2 - 25);
	}
	if (!gameStarted)
	{
		gfx.DrawStartGame(gfx.ScreenWidth / 2 - 25, gfx.ScreenHeight / 2 - 25);
	}
}
