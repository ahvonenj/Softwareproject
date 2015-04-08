#pragma once
#include <string>
#include <iostream>
#include <SFML/Network.hpp>
#include <sstream>
#include <vector>
#include "ScoreStatus.h"

using namespace sf;
using namespace std;


class Score{

public:
	Score();
	Score(string playername, int score);
	~Score(){};
	int getScore();
	string getPlayerName();
	void setPlayerName(string string);
	void setScore(int score);
	void addScore(int score);
	void printResults();
	void resetStats();
	void sendScore();
	void parsePlayerName();
	void setupBannedChars();
	static void setIsSent(bool isSent);
	static bool getIsSent();
	static ScoreStatus getSentCode();
	void setVersionHash(string hash);

private:

	string m_playername;
	int m_score;
	Http http;
	Http::Request request;
    Http::Response response;
	string m_auth;
	char bannedChars[15];
	static bool isSent;
	static ScoreStatus sentCode;
	//static int sentCode;
};
