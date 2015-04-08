#pragma once

#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include "TileType.h"
#include "TextureManager.h"
#include <iostream>
#include "TileType.h"

using namespace sf;
using namespace std;

namespace SubTileType
{
	enum subType { Default, Obsidian };
}

class Tile : public Sprite
{

public:


private:
	TextureManager* tm;
	TileType type;
	Texture tLava;  //
	Texture tGround; //lis‰tty paskaks work-aroundiks noi 2 texturee, compiles kest‰‰ vitusti kauemmin
	int subType;

	int graphicsQuality;

public:
	Tile();
	Tile(TileType tiletype, TextureManager *tm, int graphicsQuality);
	TileType getType();
	void changeType(TileType type);
	void changeColor(Color color);
	void setSubtype(int subType);
	int getSubType();
	~Tile();

private:

};

