#pragma once

#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include "TextureManager.h"
#include <iostream>
#include <math.h>
#include "Entity.h"

using namespace sf;
using namespace std;

class WaterProjectile : public CircleShape, public Entity
{

public:


private:
	float lifetime; //Elinaika(ms), 20-60 aika hyv‰
	float pAccel; //Projectilen kiihtyvyys
	float pVel_x; //x-suuntainen nopeus
	float pVel_y; //y-suuntainen nopeus
	float pVelMax; //maksiminopeus
	float radiusMax; //Projectilen radius kasvaa/pienenee, koska efekti. T‰‰ kertoo maksimiradiuksen.
	Vector2f moveDirection; //Mihin suuntaan projectile lent‰‰
	float radiusMultiplier;

public:
	WaterProjectile(float radius, TextureManager* tm, Vector2f spawnPosition, Vector2f moveDirection, float playerRotation, int graphicsQuality);
	~WaterProjectile();
	void update(Time time);
	bool getAlive();
	Sprite getShadow();

private:
	void updatePosition();
};

