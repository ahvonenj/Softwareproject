#pragma once

#include "Score.h"
#include "Scene.h"
#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include "Player.h"
#include "World.h"
#include "TextureManager.h"
#include <string>
#include <sstream>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <iomanip>
#include "EntityManager.h"
#include "WaterProjectile.h"
#include "SfxManager.h"
#include "HitEffect.h"
#include <Windows.h>
#include "IniReader.h"

using namespace sf;

class Game : public Scene
{

public:
	Game(char agrv, RenderWindow *rw, string versionHash, char playercnt);
	~Game();
	virtual int Run();
private:

	string version;
	RenderWindow *rw;
	static const Time TimePerFrame;
	Time fpsUpdateTime;
	std::size_t fpsUpdateFrames;
	Text fpsText;
	Text timeText;
	Text ammoText;
	Text healthText;
	Text pauseText;
	Text lavaGroundRatioText;
	Text ScoreText;

	Text p2ammoText;
	Text p2healthText;

	Font font;
	RectangleShape* background;
	Player* playa;
	Player* playa2;
	View view;
	World* world;
	TextureManager* tm;
	Texture background_a;
	Sprite GUIhead;
	Sprite P2GUIhead;
	Clock* totalElapsedTime;
	bool isRunning;
	float endTime;
	EntityManager<WaterProjectile*>* projectilemanager;
	SfxManager* sm;
	bool endTimeSet;
	int rotDir;
	Music music;
	int shakeAmount;
	bool lockMouse;
	bool isPaused;
	bool hitEffectPending;
	bool scoreBeenSet;
	int graphicsQuality;
	int parallaxAmount;

	Shader shader;
	RenderStates states;
	Clock shaderclock; //tarvitaan shaderiin at the moment.

	RectangleShape* backgroundB;
	Texture background_b;
	
	RectangleShape* stageShadow;
	HitEffect hiteffect;

	Time hitEffectTime;
	Score score;
	sf::String playerInputString;
	Text playerInputText;
	Time timePaused;
	Vector2f lastPos;
	bool rectSet;

	string versionHash;

	std::vector<Player*> playerVec;

	bool isMultiplayer;

	IniReader inireader;

	int gameType;

public:
	void setGametype(int gameType);


private:
	void update(Time time);
	bool processEvents();
	void render();
	void updateFps(Time time);
	void handleInput();
	void updateTime();
	void loadResourcesToMemory();
	void updateAmmo();
	void updateHealth();
	void updateScore();
	void updateTextInput();
	void setupGame();
	void updateLavaGroundRatio();

	void applyParallax();
	void updatePlayer(Time time);
	void updateProjectiles(Time time);
	void updateMusic();
	void shakeView();

	void drawBackground();
	void drawEntities();
	void drawTexts();
	void Restart();
	void resetTimeText();
	void resetScoreText();
	void checkMultiplayer();
	void manageGUIHeads();
	void customSettings();
	void defaultSettings();
};

