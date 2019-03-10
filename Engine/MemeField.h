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
		};
		//Desenha o Tile na tela
		void draw(Graphics & gfx) const
		{
			switch (state)
			{
			case MemeField::Hidden:
				SpriteCodex::DrawTileButton({ screenCoords.xCoord, screenCoords.yCoord }, gfx);
				break;

			case MemeField::Flagged:
				SpriteCodex::DrawTileButton({ screenCoords.xCoord, screenCoords.yCoord }, gfx);
				SpriteCodex::DrawTileFlag({ screenCoords.xCoord, screenCoords.yCoord }, gfx);
				break;

			case MemeField::Revealed:				
				if (hasMeme)
				{
					SpriteCodex::DrawTileBomb({ screenCoords.xCoord, screenCoords.yCoord }, gfx);
				}
				else
				{
					SpriteCodex::DrawTile0({ screenCoords.xCoord, screenCoords.yCoord }, gfx);
				}
				break;

			default:
				break;
			}
		};

		//Gera um Meme dentro do Tile
		void spawnMeme() {
			hasMeme = true;
		}
		//Retorna se o tile cont�m ou n�o um meme
		bool containMeme() const
		{
			return hasMeme;
		};

		//Abre o tile e revela seu conte�do
		void reveal()
		{	
			//assert(!state == TileState::Revealed);
			state = TileState::Revealed;
		};
		//Retorna se o tile est� fechado ou aberto.
		bool isHidden()
		{
			return state == TileState::Hidden;
		};

		//Marca o tile com um bandeira
		void flag()
		{
			state = TileState::Flagged;
		};
		//Retorna se o tile est� ou n�o marcado
		bool isFlagged()
		{
			return state == TileState::Flagged;
		};

	private:
		Vec2D gridCoords;//Coordenadas dentro do campo
		Vec2D screenCoords;//Coordenadas dentro da tela
		TileState state;//Estado atual do Tile
		bool hasMeme;//Tile cont�m ou n�o um meme
	};

public:
	MemeField() = default;
	MemeField(int _colunms, int _lines, int _nMemes);
	//@_collunms: numero de colunas do campo Memado
	//@_lines: n�mero de linhas do campo Memado
	//@_nMemes: total de memes no campo Memado	

	void drawField(Graphics & gfx);

	//revela um tile do campo
	void revealTile(const Vec2D gridCoord);
	//Marca um tile do campo
	void flagTile(const Vec2D gridCoord);

	//Retorna um tile do campo.
	Tile & tileAt(const int _x, const int _y);
	Tile & tileAt(Vec2D _gridCoords);
	const Tile & tileAt(const int _x, const int _y) const;
	const Tile & tileAt(Vec2D _gridCoords) const;
	
	//Converte cordenadas de tela em coordenadas de campo
	Vec2D screenToGridCoords(const Vec2D screenCoords) const;

	//Retorna as coordenadas de tela do campo
	Vec2D & getScreenPosition();

	//Retorna as dimens�es em pixel do campo
	Vec2D & getDimension();

private:	

	static constexpr int maxColunms = Graphics::ScreenWidth / SpriteCodex::tileSize; //M�ximo de colunas de tiles possivel 
	static constexpr int maxLines = Graphics::ScreenHeight / SpriteCodex::tileSize; // M�ximo de linhas Possiveis;
	int colunms;//N�mero de colunas da inst�ncia atual do campo
	int lines;//N�mero de linhas da inst�ncia atual do campo
	int widthInPixels;//Largura do Campo Memado em pixels
	int heightInPixels;//Altura do Campo Memado em pixels
	Vec2D screenPosition;//Posi��o do campo na tela
	Vec2D dimensionInPixels;//Dimens�es do campo em Pixels
	Tile memeTiles[maxColunms * maxLines];//Array de Tiles que formam o campo
	
	//Converte coordenadas de Campo em coordenadas de tela
	Vec2D gridToScreenCoords(const Vec2D gridCoords) const;
};
