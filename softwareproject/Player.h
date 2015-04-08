#pragma once
#define _USE_MATH_DEFINES

#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <string>
#include <iostream>
#include "EntityManager.h"
#include "WaterProjectile.h"
#include "TextureManager.h"
#include "SfxManager.h"
#include <math.h>
#include "IniReader.h"
#include <stdlib.h>
#include <stdio.h>

using namespace sf;
using namespace std;

class Player : public Sprite
{

public:

private:
	int pHealth; //Joku health
	float pAccel; //0.20f decent
	float pVel_x; //x-suuntainen velocity
	float pVel_y; //y-suuntainen velocity
	float pVelMax; //4.5f decent
	float deccelerationMultiplier; //1.2f decent
	bool isAlive;
	float fireSpeed; //millisekunteina ampumisnopeus
	int watergunAmmo;
	bool godMode;
	bool isHit;
	int graphicsQuality;

	Texture texture;
	Sprite shadow;
	EntityManager<WaterProjectile*>* projectilemanager;
	TextureManager* tm;
	SfxManager* sm;
	RenderWindow* rw;
	FloatRect drawDist;

	Sound pSfx;

	Vector2f p_realtimePosition;
	int playerid;
	Clock shootTimer;
	bool canShoot;
	Time playerUpdateTime;
Time healthRegenTime;

public:
	Player(EntityManager<WaterProjectile*>* projectilemanager, TextureManager* tm, RenderWindow* rw, SfxManager* sf, int graphicsQuality, int playerid);
	~Player();

	Vector2f getSpeed();
	void move(Vector2f dir);
	void shoot();
	void update(Time time);
	bool getAlive();
	void setAlive(bool alive);	
	void faceMouse();
	int getAmmo();
	void setAmmo(int ammo);
	bool getGodmode();
	Sprite getShadow();
	void updateShadow();
	int getHealth();
	void setHealth(int health);
	void hit(int amount);
	FloatRect getDrawDist();
	int pHealthMax;
	
	Vector2f getRealtimePosition();
	bool getIsHit();
	void setIsHit(bool isHit);
	void Reset();
	int getPlayerid();

	float joystickX;
	float joystickY;

	float joystickR;
	float joystickU;

	void customSettings(IniReader* inireader);
	void defaultSettings();

private:
	float AngleBetweenVectors(Vector2f vX, Vector2f vY);
	Vector2f MofVN(Vector2f vX, Vector2f vY);
	float lenghtOfVectors(Vector2f vX, Vector2f vY);
	void isPlayerOutOfBounds();
	void calculateVelocity();
};

