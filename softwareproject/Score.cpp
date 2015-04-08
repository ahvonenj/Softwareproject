#include "Score.h"
#include <algorithm>
#include <stdio.h>
#include "md5.h"

bool Score::isSent = false;
//int Score::sentCode = 0;

ScoreStatus Score::sentCode = DefaultStatus;

Score::Score() :
	m_score(0),
	m_playername(""),
	http("http://student.labranet.jamk.fi")
{
	setupBannedChars(); //alustetaa kielletyt charit
}

Score::Score(std::string playername, int score) :
	m_score(score),
	m_playername(playername)
{
}
//perus getit ja setit
int Score::getScore()
{
	return m_score;
}

string Score::getPlayerName()
{
	return m_playername;
}

void Score::setPlayerName(string string)
{
	m_playername = string;
}

void Score::printResults()
{
	std::cout << m_score << endl;
}


void Score::setScore(int score)
{
	m_score = score;
}

void Score::addScore(int score)
{
	m_score += score;
}

void Score::setVersionHash(string hash)
{
	this->m_auth = hash;
}
//Scoren nolaus
void Score::resetStats()
{
	m_score = 0;
	m_playername = "";
}

void Score::sendScore()
{
	parsePlayerName(); //parsitaan pelaajan nimest‰ tuhmat charit pois.
	request.setMethod(Http::Request::Post); //l‰hete‰‰n post-viesti
    request.setUri("/~G7902/softwareproject_networking/softwareproject_scoresystem/save.php");

	ostringstream stream;
	stream.str("");
	stream.clear();

	// The 13,333,337th prime is 243,316,681.
	string antihack = md5(to_string( pow(m_score, 243316681) )); //otetaan md5 scoresta ja 13,333,337thnnest‰ primest‰
	
	stream << "&name=" << m_playername << "&score=" << m_score << "&auth=" << m_auth << "&antihack=" << antihack; //tungetaan koko hˆsk‰ stringstreamiin, eli pelaajan nimi, pisteet, autentikaatio ja antihack
    request.setBody(stream.str());

	//cout << stream.str() << endl;
	Score::setIsSent(true);

	

	if(m_playername.find_first_not_of(' ') != std::string::npos && m_playername != "") //pelaaja ei saa l‰hett‰‰ tyhj‰‰ nimee
	{
		response = http.sendRequest(request); //katotaan mit‰ POST-viestin response on

		if(response.getField("Debug1") != "")
		{
			cout << response.getField("Debug1") << endl;
		}
		if(response.getField("Debug2") != "")
		{
			cout << response.getField("Debug2") << endl;
		}
		if(response.getField("Debug3") != "")
		{
			cout << response.getField("Debug3") << endl;
		}

		cout << "_______________________________________" << endl;
	}
	else
	{
		Score::sentCode = InvalidName;
	}

	if (response.getStatus() == Http::Response::Ok)
	{
		Score::sentCode = Success;
	}

	else if (response.getStatus() == Http::Response::NotFound)
	{
		Score::sentCode = ServerError;
	}

	else if (response.getStatus() == Http::Response::InternalServerError)
	{
		std::cout << "Internal Server Error > " << std::cout << response.getStatus() << std::endl;
		Score::sentCode = ServerError;
	}
	else
	{
		std::cout << response.getStatus() << std::endl;
		Score::sentCode = InvalidName;
	}

	if(response.getField("Success") != "")
	{
		if(stoi(response.getField("Success")) == 0)
		{
			Score::sentCode = ClientOutdated;
		}
	}

}

//parsitaan kielletyt characterit pois pelaajan nimest‰
void Score::parsePlayerName()
{
	for (int i = 0; i < 12; i++)
	{
		if (m_playername.find(bannedChars[i]) != std::string::npos)
		{
			m_playername.erase(std::remove(m_playername.begin(),m_playername.end(), bannedChars[i]),  m_playername.end() );
		}
	}
}

void Score::setIsSent(bool isSent)
{
	Score::isSent = isSent;
}

bool Score::getIsSent()
{
	return Score::isSent;
}

ScoreStatus Score::getSentCode()
{
	return Score::sentCode;
}

void Score::setupBannedChars()
{
    bannedChars[0] = '|';
	bannedChars[1] = '<';
	bannedChars[2] = '>';
	bannedChars[3] = '"';
	bannedChars[4] = '(';
	bannedChars[5] = ')';
	bannedChars[6] = '{';
	bannedChars[7] = '}';
	bannedChars[8] = ';';
	bannedChars[9] = ',';
	bannedChars[10] = '/';
	bannedChars[11] = '\\';
	bannedChars[12] = '$';
}