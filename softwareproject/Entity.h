#pragma once
#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include "TextureManager.h"

using namespace sf;
 //entityjä on eri pelimaailman objektit, powerupit, projektiilit...
class Entity
{

protected:
	float lifetime;
	TextureManager* texturemanager;
	bool isAlive;
	Sprite shadow;
	int graphicsQuality;

public:
	virtual void update(Time time) = 0;
	bool getAlive();
	void setAlive(bool alive);
	Entity(TextureManager* texturemanager, int graphicsQuality);
	void drawShadow(RenderWindow* rw);
	void updateEntityShadow(Vector2f newPosition, float newRotation, float shadowSpan = 1.f);
	~Entity(void);
};

