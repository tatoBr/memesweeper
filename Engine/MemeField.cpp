#include "MemeField.h"

MemeField::MemeField(int _colunms, int _lines, int _nMemes)
{
	assert(_nMemes < (_colunms * _lines) - 1);
	assert(_colunms <= maxColunms && _lines <= maxLines);

	colunms = _colunms;
	lines = _lines;
	widthInPixels = colunms * SpriteCodex::tileSize;
	heightInPixels = lines * SpriteCodex::tileSize;

	screenPosition = { (Graphics::ScreenWidth - widthInPixels) / 2 , (Graphics::ScreenHeight - heightInPixels) / 2 };
	dimensionInPixels = { colunms * SpriteCodex::tileSize, lines * SpriteCodex::tileSize};
	
	//Inicializa os Tiles
	for (int lin = 0; lin < lines; lin++)
	{
		for (int col = 0; col < colunms; col++)		
		{
			const int index = lin * colunms + col;
			memeTiles[index].startUpTile({ col, lin }, gridToScreenCoords({ col, lin }));
		}
	}
	std::random_device rd;
	std::mt19937 range(rd());
	std::uniform_int_distribution<int> xDist(0, colunms);
	std::uniform_int_distribution<int> yDist(0, lines);

	//Gera os memes aleatoriamente dentro do MemeField
	do {
		Vec2D newPos = {xDist(range), yDist(range)};
		if (!tileAt(newPos).containMeme())
		{
			tileAt(newPos).spawnMeme();
			_nMemes--;
		}

	} while (_nMemes > 0);

	for (int y = 0; y < lines; y++)
	{
		for (int x = 0; x < colunms; x++)
		{
			if (!tileAt(x, y).containMeme()) {
				Vec2D gp = { x, y };
				tileAt(x, y).setAdjacentCount(countAdjacentMemes(gp));
			}
		}
	}
	
}

void MemeField::drawField(Graphics & gfx)
{
	const int top = screenPosition.yCoord;
	const int left = screenPosition.xCoord;
	const int botton = top + (lines * SpriteCodex::tileSize);
	const int right = left + (colunms * SpriteCodex::tileSize);

	gfx.DrawRect({ { left, top }, { right, botton } }, Color(178, 178, 178));

	for (int lin = 0; lin < lines; lin++)
	{
		for (int col = 0; col < colunms; col++)
		{
			const int index = lin * colunms + col;
			memeTiles[index].draw(gfx, blownUp);
		}
	}
}

void MemeField::revealTile(const Vec2D _gridCoord)
{	
	if (tileAt(_gridCoord).isHidden() && !tileAt(_gridCoord).isFlagged() && !blownUp)
	{
		tileAt(_gridCoord).reveal();
		if (tileAt(_gridCoord).containMeme())
		{
			blownUp = true;
		}
	}
}

void MemeField::toggleFlag(const Vec2D _gridCoord)
{
	if (tileAt(_gridCoord).isHidden() && !blownUp)
	{
		tileAt(_gridCoord).flag();
	}
	else if (tileAt(_gridCoord).isFlagged() && !blownUp)
	{
		tileAt(_gridCoord).unflag();
	}
}

MemeField::Tile & MemeField::tileAt(const int _x, const int _y)
{
	return memeTiles[_y * colunms + _x];
}
MemeField::Tile & MemeField::tileAt(Vec2D _gridCoords)
{
	return tileAt(_gridCoords.xCoord, _gridCoords.yCoord);
}
const MemeField::Tile & MemeField::tileAt(const int _x, const int _y) const
{
	return memeTiles[_y * colunms + _x];
}
const MemeField::Tile & MemeField::tileAt(Vec2D _gridCoords) const
{
	return tileAt(_gridCoords.xCoord, _gridCoords.yCoord);
}

int MemeField::countAdjacentMemes(Vec2D & _gridCoord)
{

	const int xStart = std::max(0 , _gridCoord.xCoord - 1);
	const int xEnd = std::min(colunms - 1, _gridCoord.xCoord + 1);

	const int yStart = std::max(0, _gridCoord.yCoord - 1);
	const int yEnd = std::min(lines - 1, _gridCoord.yCoord + 1);

	int nMemes = 0;

	for (int y = yStart; y <= yEnd; y++) {
		for (int x = xStart; x <= xEnd; x++)
		{			
			if (tileAt(x, y).containMeme())
			{
				nMemes++;
			}					
		}
	}

	return nMemes;
}

MemeField::Vec2D MemeField::screenToGridCoords(const Vec2D _screenCoords) const
{
	int gridX = ((_screenCoords.xCoord - screenPosition.xCoord) / SpriteCodex::tileSize);
	int gridY = ((_screenCoords.yCoord - screenPosition.yCoord) / SpriteCodex::tileSize);
	return { gridX, gridY };
}

MemeField::Vec2D & MemeField::getScreenPosition()
{
	return  screenPosition;
}
MemeField::Vec2D & MemeField::getDimension()
{
	return dimensionInPixels;
}
MemeField::Vec2D MemeField::gridToScreenCoords(const Vec2D gridCoords) const
{
	const int screenX = screenPosition.xCoord + (gridCoords.xCoord * SpriteCodex::tileSize);
	const int screenY = screenPosition.yCoord + (gridCoords.yCoord * SpriteCodex::tileSize);
	return {screenX, screenY};
}
