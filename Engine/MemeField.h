#pragma once

#include "Graphics.h"
#include "SpriteCodex.h"
#include <random>
#include <assert.h>

class MemeField {
public:
	//Estrutura simples para guardar Coordenadas 2D
	struct Vec2D {
		int xCoord;
		int yCoord;
	};

private:
	//Enumerador contendo todos os estados possiveis de um Tile;
	enum TileState : unsigned char {
		Hidden,
		Flagged,
		Revealed		
	};

	//Classe Tile Privada. Visivel somente para memesField
	class Tile {		
	public:
		Tile() = default;
		
		//Inicializa o Tile 
		void startUpTile(const Vec2D _gridCoord, const Vec2D _screenCoords)
		{			
			gridCoords = _gridCoord;
			screenCoords = _screenCoords;

			state = TileState::Hidden;
			hasMeme = false;
			adjacentMemesCount = -1;
		};

		//Desenha o Tile na tela
		void draw(Graphics & gfx, bool _blownUp) const
		{
			switch (state)
			{
			case MemeField::Hidden:
				if (hasMeme && _blownUp)
				{
					SpriteCodex::DrawTileBomb({ screenCoords.xCoord, screenCoords.yCoord }, gfx);
				}
				else {
					SpriteCodex::DrawTileButton({ screenCoords.xCoord, screenCoords.yCoord }, gfx);
				}
				break;

			case MemeField::Flagged:
				if (hasMeme && _blownUp)
				{
					SpriteCodex::DrawTileBomb({ screenCoords.xCoord, screenCoords.yCoord }, gfx);
					SpriteCodex::DrawTileFlag({ screenCoords.xCoord, screenCoords.yCoord }, gfx);
				}
				else if (!hasMeme && _blownUp)
				{
					SpriteCodex::DrawTileFlag({ screenCoords.xCoord, screenCoords.yCoord }, gfx);
					SpriteCodex::DrawTileCross({ screenCoords.xCoord, screenCoords.yCoord }, gfx);
				}
				else
				{
					SpriteCodex::DrawTileButton({ screenCoords.xCoord, screenCoords.yCoord }, gfx);
					SpriteCodex::DrawTileFlag({ screenCoords.xCoord, screenCoords.yCoord }, gfx);
				}
				break;

			case MemeField::Revealed:				
				if (hasMeme && _blownUp)
				{
					SpriteCodex::DrawTileBombRed({ screenCoords.xCoord, screenCoords.yCoord }, gfx);
				}
				else
				{
					drawNumberTile(gfx);
				}

			default:
				break;
			}
		};

		//Gera um Meme dentro do Tile
		void spawnMeme() {
			hasMeme = true;
		}
		//Retorna se o tile contém ou não um meme
		bool containMeme() const
		{
			return hasMeme;
		};

		void setAdjacentCount(const int _adjacentMemes)
		{
			adjacentMemesCount = _adjacentMemes;
		}

		//Abre o tile e revela seu conteúdo
		void reveal()
		{	
			//assert(!state == TileState::Revealed);
			state = TileState::Revealed;
		};
		//Retorna se o tile está fechado ou aberto.
		bool isHidden()
		{
			return state == TileState::Hidden;
		};

		//Marca e desmarca o tile com um bandeira
		void flag()
		{
			assert(state != TileState::Flagged);
			state = TileState::Flagged;
		};
		void unflag()
		{
			assert(state == TileState::Flagged);
			state = TileState::Hidden;
		}
		//Retorna se o tile está ou não marcado
		bool isFlagged()
		{
			return state == TileState::Flagged;
		};

	private:
		Vec2D gridCoords;//Coordenadas dentro do campo
		Vec2D screenCoords;//Coordenadas dentro da tela
		TileState state;//Estado atual do Tile
		bool hasMeme;//Tile contém ou não um meme
		int adjacentMemesCount = -1;//Memes existentes nos tiles vizinhos

		void drawNumberTile(Graphics & gfx) const
		{
			switch (adjacentMemesCount)
			{
			case 0:
				SpriteCodex::DrawTile0({ screenCoords.xCoord, screenCoords.yCoord }, gfx);
				break;

			case 1:
				SpriteCodex::DrawTile1({ screenCoords.xCoord, screenCoords.yCoord }, gfx);
				break;

			case 2:
				SpriteCodex::DrawTile2({ screenCoords.xCoord, screenCoords.yCoord }, gfx);
				break;

			case 3:
				SpriteCodex::DrawTile3({ screenCoords.xCoord, screenCoords.yCoord }, gfx);
				break;

			case 4:
				SpriteCodex::DrawTile4({ screenCoords.xCoord, screenCoords.yCoord }, gfx);
				break;

			case 5:
				SpriteCodex::DrawTile5({ screenCoords.xCoord, screenCoords.yCoord }, gfx);
				break;

			case 6:
				SpriteCodex::DrawTile6({ screenCoords.xCoord, screenCoords.yCoord }, gfx);
				break;

			case 7:
				SpriteCodex::DrawTile7({ screenCoords.xCoord, screenCoords.yCoord }, gfx);
				break;

			case 8:
				SpriteCodex::DrawTile8({ screenCoords.xCoord, screenCoords.yCoord }, gfx);
				break;
			
			default:
				SpriteCodex::DrawTile0({ screenCoords.xCoord, screenCoords.yCoord }, gfx);
				break;
			}
		}		
	};

public:
	MemeField() = default;
	MemeField(int _colunms, int _lines, int _nMemes);
	//@_collunms: numero de colunas do campo Memado
	//@_lines: número de linhas do campo Memado
	//@_nMemes: total de memes no campo Memado	

	void drawField(Graphics & gfx);

	//revela um tile do campo
	void revealTile(const Vec2D gridCoord);
	//Marca um tile do campo
	void toggleFlag(const Vec2D gridCoord);

	//Retorna um tile do campo.
	Tile & tileAt(const int _x, const int _y);
	Tile & tileAt(Vec2D _gridCoords);
	const Tile & tileAt(const int _x, const int _y) const;
	const Tile & tileAt(Vec2D _gridCoords) const;

	int countAdjacentMemes(Vec2D & _gridCoord);
	
	//Converte cordenadas de tela em coordenadas de campo
	Vec2D screenToGridCoords(const Vec2D screenCoords) const;

	//Retorna as coordenadas de tela do campo
	Vec2D & getScreenPosition();

	//Retorna as dimensões em pixel do campo
	Vec2D & getDimension();

private:	

	static constexpr int maxColunms = Graphics::ScreenWidth / SpriteCodex::tileSize; //Máximo de colunas de tiles possivel 
	static constexpr int maxLines = Graphics::ScreenHeight / SpriteCodex::tileSize; // Máximo de linhas Possiveis;

	int colunms;//Número de colunas da instância atual do campo
	int lines;//Número de linhas da instância atual do campo

	int widthInPixels;//Largura do Campo Memado em pixels
	int heightInPixels;//Altura do Campo Memado em pixels

	bool blownUp = false;

	Vec2D screenPosition;//Posição do campo na tela	
	Vec2D dimensionInPixels;//Dimensões do campo em Pixels	
	Tile memeTiles[maxColunms * maxLines];//Array de Tiles que formam o campo
	
	//Converte coordenadas de Campo em coordenadas de tela
	Vec2D gridToScreenCoords(const Vec2D gridCoords) const;
};
