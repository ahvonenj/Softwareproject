#pragma once

#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include "Entity.h"

using namespace sf;

class Powerup : public Sprite, public Entity
{

public:
	virtual void pickup(int ind) = 0;
	virtual void update(Time time) = 0;
	void spawn(Vector2f spawnpoint, float lifetime);
	Powerup(TextureManager* texturemanager, int graphicsQuality);
	~Powerup();

private:
	bool hasSpawned;

public:
	bool getHasSpawned();
	void setHasSpawned(bool hasSpawned);

};

