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
	ball(Vec2(400.0f,540.0f), Vec2(300.0f, -300.0f)),
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
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	float deltaTime = ft.Mark();
	
	if (!gameStarted && wnd.kbd.KeyIsPressed(VK_RETURN))
	{
		gameStarted = true;
	}

	if (ball.BottomCollision(walls))
	{
		gameOver = true;
	}

	if (gameStarted && !gameOver)
	{
		ball.Update(deltaTime);
		ball.WallCollision(walls);

		for (Brick& brick : bricks)
		{
			brick.BallCollision(ball);
			brick.BallCollision(ball);
		}

		pad.BallCornerCollision(ball);
		pad.BallCollision(ball);
		pad.WallCollision(walls);
		pad.Update(deltaTime, wnd.kbd, walls);
	}
}

void Game::ComposeFrame()
{
	for (Brick& brick : bricks)
	{
		brick.Draw(gfx);
	}

	if (gameOver)
	{
		gfx.DrawGameOver(gfx.ScreenWidth / 2 - 25, gfx.ScreenHeight / 2 - 25);
	}

	if (!gameStarted)
	{
		gfx.DrawStartGame(gfx.ScreenWidth / 2 - 25, gfx.ScreenHeight / 2 - 25);
	}

	ball.Draw(gfx);
	pad.Draw(gfx);

}
