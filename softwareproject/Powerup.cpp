#include "Powerup.h"


Powerup::Powerup(TextureManager* texturemanager, int graphicsQuality) : Entity(texturemanager, graphicsQuality), hasSpawned(false)
{
}

void Powerup::spawn(Vector2f spawnPoint, float lifetime)
{
	this->setPosition(spawnPoint);
	this->lifetime = lifetime;
	this->isAlive = true;
	this->hasSpawned = true;
	this->setColor(Color(this->getColor().r, this->getColor().g, this->getColor().b, 255));
}

bool Powerup::getHasSpawned()
{
	return this->hasSpawned;
}

void Powerup::setHasSpawned(bool hasSpawned)
{
	this->hasSpawned = hasSpawned;
}

Powerup::~Powerup()
{
}