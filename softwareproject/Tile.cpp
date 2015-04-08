#include "Tile.h"

//Tiili-luokka maailmaa varten
Tile::Tile()
{
	this->type = Ground;
	this->subType = SubTileType::Default;
}

Tile::Tile(TileType tiletype, TextureManager *tm, int graphicsQuality)
{
	this->tm = tm;
	this->type = tiletype;
	this->graphicsQuality = graphicsQuality;


	if(tiletype == Lava) //jos tiili on laavaa, ni piirretään se laavaks. Jos matala grafiikka käytetää perus-spritee, muuten osittain läpynäkyvä et näkyy makeesti shader
	{

		if(graphicsQuality == 1)
		{
			this->setTexture(tm->get(Textures::Blank));
			this->setColor(Color(200, 200, 200, 75));
		}
		else
		{
			this->setTexture(tm->get(Textures::LavaTexture)); 
			this->setColor(Color(2555, 255, 255, 255));
		}
		
	}
	else if(tiletype == Ground)
	{
		this->setTexture(tm->get(Textures::GroundTexture));

		int rc = (rand()%(225 - 175))+175;

		this->setColor(Color(rc, rc, rc, 255));
		this->subType = SubTileType::Default;
	}
	else
	{		
		this->setTexture(tm->get(Textures::GroundTexture));
		this->setColor(Color::Magenta);
		this->subType = SubTileType::Default;
	}
	
}

TileType Tile::getType()
{
	return this->type;
}

//Vaihdetaan tilen tyyppiä laavasta kiveen. Muutetaan myös väriä randomilla, tulee siisti efekti ku ei oo ihan tasainen
void Tile::changeType(TileType type)
{

	if (type == TileType::Lava) {
		 
		if(graphicsQuality == 1)
		{
			this->setTexture(tm->get(Textures::Blank)); 
			this->setColor(Color(200, 200, 200, 75));
		}
		else
		{
			this->setTexture(tm->get(Textures::LavaTexture)); 
			this->setColor(Color(255, 255, 255, 255));
		}

		
	}

	if (type == TileType::Ground)
	{
		this->setTexture(tm->get(Textures::GroundTexture));

		int ra = (rand()%(195 - 155))+155;
		int rb = (rand()%(15 - 75))+75;

		this->setColor(Color(ra, rb, ra, 255));
	}
	this->type = type;
}

void Tile::changeColor(Color color)
{
	this->setColor(color);
}

void Tile::setSubtype(int subType)
{
	//if(subType == SubTileType::Default)

	this->subType = subType;
}

int Tile::getSubType()
{
	return this->subType;
}

Tile::~Tile()
{
}
