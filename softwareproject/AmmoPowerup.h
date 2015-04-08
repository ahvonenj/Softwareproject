#pragma once
#include "Powerup.h"
#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include "Player.h"

using namespace sf;

class AmmoPowerup : public Powerup
{

private:
	Player* player;
	std::vector<Player*> playerVec;
	Time powerupUpdateTime;

public:
	AmmoPowerup(vector<Player*> playerVec, TextureManager* texturemanager, int graphicsQuality);
	~AmmoPowerup();

	virtual void update(Time time);
	virtual void pickup(int ind);

};

