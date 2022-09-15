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
	soundPad(L"Sounds\\arkpad.wav"),
	soundBrick(L"Sounds\\arkbrick.wav"),
	soundReady(L"Sounds\\ready.wav"),
	soundFart(L"Sounds\\fart.wav"),
	walls(650.0f, 150.0f, 25.0f, 575.0f),
	ball(Vec2((walls.right + walls.left) / 2.0f, walls.bottom - 64.0f), Vec2(-550.0f, -550.0f)), // Change the multiplier in the "Pad.cpp" if you change the ball velocity
	pad(Vec2((walls.right + walls.left)/2.0f, walls.bottom - 50.0f), 27, 7)
{
	const Vec2 brickStartPos = Vec2(walls.left + 15.0f, walls.top + 15.0f);

	Color color1List[4] = { Colors::MakeRGB(175,0,0), Colors::MakeRGB(0,175,0), Colors::MakeRGB(0,0,175), Colors::MakeRGB(175,146,0) };
	Color color2List[4] = { Colors::Red, Colors::Green, Colors::Blue, Colors::Yellow };
	for (int y = 0; y < bricksRows ; ++y)
	{
		for (int x = 0; x < bricksColumns; ++x)
		{
			bricks[x + y * bricksColumns] = Brick(Vec2((x * brickWidth) + brickStartPos.x, (y * brickHeight) + brickStartPos.y), brickWidth, brickHeight, color1List[y % 4], color2List[y % 4]);
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
	if (ball.BottomCollision(walls))
	{
		if (lives == 1)
		{
			gameOver = true;
			if (fartPlaying)
			{
				soundFart.Play(1.0f, 0.3f);
				fartPlaying = false;
			}
		}
		else
		{
			soundFart.Play(1.0f, 0.3f);
			lives -= 1;
		}
	}

	// Checking if the game is active
	if (gameStarted && !gameOver)
	{
		// Making the ball move
		ball.Update(deltaTime);

		// Calculating the ball/outer-walls collision
		if (ball.WallCollision(walls))
		{
			soundPad.Play(1.0f, 0.3f);
		}

		// Making the ball hit the closest colliding brick 
		int currentColIndex;
		float currentColDistSq;
		bool hasCollided = false;
		for (int i = 0; i < nBricks; i++)
		{
			if (bricks[i].CheckBallCollision(ball))
			{
				soundBrick.Play(1.0f, 0.3f);
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
		if (pad.BallCollision(ball, wnd.kbd))
		{
			soundPad.Play(1.0f, 0.3f);
		}
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
	for (int i = 0; i < lives; i++)
	{
		if (!gameOver)
		{
			gfx.DrawHeart(int(walls.right - 30.0f - i * 30.0f), int(walls.bottom - 30.0f));
		}
	}

	gfx.DrawWalls(walls, 5.0f, Colors::Blue);

	for (Brick& brick : bricks)
	{
		brick.Draw(gfx);
	}
	ball.Draw(gfx);
	pad.Draw(gfx, pad.GetCenter());

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
