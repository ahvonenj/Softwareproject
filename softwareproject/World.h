#pragma once

#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include "Score.h"
#include <vector>
#include "Tile.h"
#include "TextureManager.h"
#include <fstream>
#include <iostream>
#include "TileType.h"
#include "Player.h"
#include "SfxManager.h"
#include "AmmoPowerup.h"
#include <ctime>
#include <random>
#include "IniReader.h"

using namespace sf;
using namespace std;

const int WIDTH = 32;
const int HEIGHT = 32;
const int TILESIZE = 25;

class World : public NonCopyable
{

public:
	

private:

	char charmap2D[WIDTH][HEIGHT];
	Tile tilemap2D[WIDTH][HEIGHT];
	RenderWindow* rw;
	TextureManager* tm;
	EntityManager<WaterProjectile*>* projectilemanager;
	Player* player;
	SfxManager* sm;
	Score* score;

	Sound sfx;
	SoundBuffer buffer;

	int lavaSpreadChance;
	int pickupChance;

	int currentLavaTiles;
	int currentGroundTiles;

	int graphicsQuality;

	EntityManager<Powerup*>* powerupmanager;
	random_device dev;

	int r;
	Time worldUpdateTime;
	int floodDir;

	bool spawnEnabled;
	std::vector<Player*> playerVec;

public:
	//World(RenderWindow* rw, TextureManager& tm, Player* player, EntityManager<WaterProjectile*>* projectilemanager, SfxManager* sm, Score& score,int graphicsQuality);
	World(RenderWindow* rw, TextureManager& tm, vector<Player*>& playerVec, EntityManager<WaterProjectile*>* projectilemanager,
		SfxManager* sm, Score& score,int graphicsQuality);

	~World();

	void update(Time time);
	void draw();
	int getCurrentLavaTiles();
	int getCurrentGroundTiles();
	void Reset();
	void enableSpawn();
	bool isSpawnEnabled();
	void customSettings(IniReader* inireader);
	void defaultSettings();

private:

	bool playerPinpointIntersects(Vector2i pinpointLocation);
	bool projectilePinpointIntersects(Vector2i pinpointLocation, WaterProjectile* projectile);
	void buildStage();
	void initalizeWorld();
	int getWidth();
	int getHeight();
	void floodByLava();
	void floodByGround();
	void floodTile(int i, int j, int method);
	void tileColorsToDefaults();
};

