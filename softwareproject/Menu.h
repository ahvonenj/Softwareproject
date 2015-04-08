#pragma once

#include "Scene.h"
#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include <iostream>
#include "Score.h"
#include <SFML/Network.hpp>
#include <sstream>
#include <Windows.h>

using namespace sf;
using namespace std;

class Menu : public Scene
{
public:
	Menu(RenderWindow *rw, string versionHash);
	virtual int Run();
	~Menu(){};
private:
	void fadeOut();
	void fadeIn();
	int processMenuEvents();
	void draw();
	void adjustButtons();
	void updateScoreStatus();
	void checkForNewVersion();


public:

private:

	RenderWindow *rw;
	RectangleShape backgroundPic;
	RectangleShape newButton;
	RectangleShape quitButton;
	RectangleShape scoreButton;
	RectangleShape customButton;

	Texture bg;
	Texture newgame;
	Texture quit;
	Texture highscore;
	Texture customgame;

	RectangleShape fading;

	Music theme; 
	bool isRunning;
	int menuselection;

	float accumulator;
	float transparency;

	Text scoreInfoText;
	Text versionCheckText;
	Font font;
	ScoreStatus code;

	string versionHash;
	bool newVersionReleased;
	Http http;
	Http::Request request;
    Http::Response response;

	string downloadLink;

};