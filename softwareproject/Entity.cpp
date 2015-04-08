#include "Entity.h"

Entity::Entity(TextureManager* texturemanager, int graphicsQuality)
{
	this->texturemanager = texturemanager;
	this->graphicsQuality = graphicsQuality;
}

void Entity::update(Time time)
{

}

void Entity::updateEntityShadow(Vector2f newPosition, float newRotation, float shadowSpan)
{
	this->shadow.setPosition(newPosition.x + shadowSpan, newPosition.y + shadowSpan);
	this->shadow.setRotation(newRotation);
}

bool Entity::getAlive()
{
	return isAlive;
}

void Entity::setAlive(bool alive)
{
	this->isAlive = alive;
}

void Entity::drawShadow(RenderWindow* rw)
{
	if(graphicsQuality == 1)
		rw->draw(this->shadow);
}

Entity::~Entity(void)
{
}
