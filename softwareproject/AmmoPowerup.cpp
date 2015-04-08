#include "AmmoPowerup.h"
AmmoPowerup::AmmoPowerup(vector<Player*> playerVec, TextureManager* texturemanager, int graphicsQuality) : Powerup(texturemanager, graphicsQuality)
{
	this->playerVec = playerVec;
	this->setTexture(texturemanager->get(Textures::Droplet));
	this->setScale(0.78f, 0.78f);
	this->setColor(Color(255,255,255, 0));
	this->isAlive = false;

	
	this->setOrigin(this->getTexture()->getSize().x / 2, this->getTexture()->getSize().y / 2);
	this->rotate(rand() % 360);

	this->shadow.setTexture(texturemanager->get(Textures::Droplet));
	this->shadow.setScale(0.78f, 0.78f);
	this->shadow.setColor(Color(40, 40, 40, 100));
	this->shadow.setRotation(this->getRotation());
	this->shadow.setOrigin(this->getTexture()->getSize().x / 2, this->getTexture()->getSize().y / 2);
	this->shadow.setPosition(this->getPosition().x + 1, this->getPosition().y + 1);
	
}

// Ammopowerupin päivitys
void AmmoPowerup::update(Time time)
{
	
	powerupUpdateTime += time;

	if(isAlive)
	{
		if(powerupUpdateTime >= milliseconds(10.f))
		{
			lifetime -= 1.f;
		}
	}

	updateEntityShadow(this->getPosition(), this->getRotation(), 3.5f);
	if(lifetime <= 0.f)
	{
		this->isAlive = false;
	}
}

//Ammopowerupin nostaminen
void AmmoPowerup::pickup(int ind)
{
	playerVec[ind]->setAmmo(playerVec[ind]->getAmmo() + 20);
	this->isAlive = false;
}

AmmoPowerup::~AmmoPowerup(void)
{
}
