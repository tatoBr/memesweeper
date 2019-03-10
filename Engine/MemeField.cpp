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
			memeTiles[index].draw(gfx);
		}
	}
}

void MemeField::revealTile(const Vec2D _gridCoord)
{	
	if (tileAt(_gridCoord).isHidden() && !tileAt(_gridCoord).isFlagged())
	{
		tileAt(_gridCoord).reveal();
	}
}

void MemeField::flagTile(const Vec2D _gridCoord)
{
	if (!tileAt(_gridCoord).isFlagged() && tileAt(_gridCoord).isHidden())
	{
		tileAt(_gridCoord).flag();
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
