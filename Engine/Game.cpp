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
	mf(10,18,15)
{
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
	while (!wnd.mouse.IsEmpty()) {
		const Mouse::Event e = wnd.mouse.Read();//Le o tipo de evento lançado pela ação do mouse

		if (e.GetType() == Mouse::Event::Type::LPress)
		{
			if (clickIsOnField(e.GetPos(), mf))
			{
				mf.revealTile(mf.screenToGridCoords({ e.GetPosX() , e.GetPosY() }));
			}
		}

		else if (e.GetType() == Mouse::Event::Type::RPress)
		{
			if (clickIsOnField(e.GetPos(), mf))
			{
				mf.toggleFlag(mf.screenToGridCoords({ e.GetPosX() , e.GetPosY() }));
			}
		}
	}
	
	
}

bool Game::clickIsOnField(const std::pair<int, int> _mousePos, MemeField & _memeField) const
{
	const int mouseX = _mousePos.first;
	const int mouseY = _mousePos.second;

	const int left = _memeField.getScreenPosition().xCoord;
	const int top = _memeField.getScreenPosition().yCoord;
	const int right = left + _memeField.getDimension().xCoord;
	const int botton = top + _memeField.getDimension().yCoord;

	return (mouseX >= left && mouseX <= right) && (mouseY >= top && mouseY <= botton);
}

void Game::ComposeFrame()
{
	mf.drawField(gfx);
}
