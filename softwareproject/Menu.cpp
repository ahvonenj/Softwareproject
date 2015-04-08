#include "Menu.h"
#include "ScoreStatus.h"

//

Menu::Menu(RenderWindow *renderwindow, string versionHash) :
	backgroundPic(Vector2f(1280,800)),
	newButton(Vector2f(300,100)),
	quitButton(Vector2f(300,100)),
	scoreButton(Vector2f(300,100)),
	customButton(Vector2f(300,100)),
	fading(Vector2f(1280,800)),
	menuselection(0),
	accumulator(0.0f),
	transparency(255.f),
	rw(renderwindow),
	newVersionReleased(false),
	http("http://student.labranet.jamk.fi"),
	versionHash(versionHash)
{
	bg.loadFromFile("Resources/volcanobg.png");
	newgame.loadFromFile("Resources/newgame.png");
	quit.loadFromFile("Resources/quit.png");
	highscore.loadFromFile("Resources/highscore.png");
	customgame.loadFromFile("Resources/customgame.png");

	bg.setSmooth(true);
	newgame.setSmooth(true);
	quit.setSmooth(true);
	highscore.setSmooth(true);
	customgame.setSmooth(true);

	newButton.setTexture(&newgame);
	quitButton.setTexture(&quit);
	scoreButton.setTexture(&highscore);
	customButton.setTexture(&customgame);
	backgroundPic.setTexture(&bg);

	newButton.setFillColor(Color(255,255,255));
	quitButton.setFillColor(Color(175,175,175));
	scoreButton.setFillColor(Color(175,175,175));
	customButton.setFillColor(Color(175,175,175));

	newButton.setPosition(800,300);
	customButton.setPosition(800,425);
	scoreButton.setPosition(800, 550);
	quitButton.setPosition(800, 675);

	font.loadFromFile("Resources/04b.ttf");

	scoreInfoText.setFont(font);
	//scoreInfoText.setPosition(820.f, 270.f);
	scoreInfoText.setPosition(3.f, 3.f);
	scoreInfoText.setColor(Color(50, 200, 50, 255));
	scoreInfoText.setCharacterSize(16.f);
	scoreInfoText.setStyle(Text::Bold);
	//scoreInfoText.setString("Your score has been sent!");

	versionCheckText.setFont(font);
	versionCheckText.setPosition(rw->getSize().x - versionCheckText.getGlobalBounds().width - 3.f, 3.f);
	versionCheckText.setColor(Color::Yellow);
	versionCheckText.setCharacterSize(16.f);
	versionCheckText.setStyle(Text::Bold);

	theme.openFromFile("Resources/intro.wav");

	isRunning = false;
	fading.setFillColor(Color(0,0,0,255));
	
}

//Menun p‰‰metodi
int Menu::Run()
{
	scoreInfoText.setString("");
	updateScoreStatus();
	checkForNewVersion();
	theme.play();
	theme.setVolume(100);
	isRunning = true;
	int menustate = 0;
	rw->setView(rw->getDefaultView());
	rw->clear();

	fadeIn();
	while(isRunning)
	{
		menustate = processMenuEvents();
		if (menustate == 1){
			fadeOut();
			theme.stop();
			return 1;
		}
		if (menustate == 2){
			fadeOut();
			theme.stop();
			return 2;
		}

		if (menustate == -1){
			return -1;
		}
		//updateScoreStatus();
		draw();
	}
	return 1;
}


//Tarkisetaan pelaajan versio
void Menu::checkForNewVersion()
{
	request.setMethod(Http::Request::Post);
    request.setUri("/~G7902/softwareproject_networking/softwareproject_versioncheck/versioncheck.php");

	ostringstream stream;
	stream.str("");
	stream.clear();
	
	stream << "&myversion=" << versionHash;
    request.setBody(stream.str());
	response = http.sendRequest(request);

	if(response.getStatus() == Http::Response::Ok)
	{
		if(response.getField("Versionreply") != "")
		{
			if(stoi(response.getField("Versionreply")) == 0)
			{
				newVersionReleased =false;

				versionCheckText.setColor(Color(50, 200, 50, 255));
				versionCheckText.setString("Running newest version of Softwareproject!");
				versionCheckText.setPosition(rw->getSize().x - versionCheckText.getGlobalBounds().width - 3.f, 3.f);
			}
			else if(stoi(response.getField("Versionreply")) == 3)
			{
					versionCheckText.setColor(Color::Red);
					versionCheckText.setString("Versioncheck failed");
					versionCheckText.setPosition(rw->getSize().x - versionCheckText.getGlobalBounds().width - 3.f, 3.f);
			}
			else
			{
				newVersionReleased = true;

				if(response.getField("Versiondownloadlink") != "")
				{
					versionCheckText.setColor(Color::Yellow);
					versionCheckText.setString("New version available at: <" + response.getField("Versiondownloadlink") + "> - Click to download");
					versionCheckText.setPosition(rw->getSize().x - versionCheckText.getGlobalBounds().width - 3.f, 3.f);
					downloadLink = response.getField("Versiondownloadlink");
				}
			}
		}
		else
		{
			versionCheckText.setColor(Color::Red);
			versionCheckText.setString("Versioncheck failed" + response.getStatus());
			versionCheckText.setPosition(rw->getSize().x - versionCheckText.getGlobalBounds().width - 3.f, 3.f);
		}
	}
	else
	{
		versionCheckText.setColor(Color::Red);
		versionCheckText.setString("Versioncheck failed" + response.getStatus());
		versionCheckText.setPosition(rw->getSize().x - versionCheckText.getGlobalBounds().width - 3.f, 3.f);
	}
}
//Tehd‰‰n makee feidaus kun menusta siirryt‰‰n toiseen tilaan.
void Menu::fadeOut()
{

	Clock clock;
	transparency = 0;
	float volume = 100;
	while (transparency <= 255)
	{
	accumulator += clock.getElapsedTime().asSeconds();
			clock.restart();

			while (accumulator >= 0.0005f && transparency >= 0)
			{
				accumulator -= 0.0005f;
				transparency+= 0.1f;
				
				
				if (volume >= 0) {
					volume-= 0.1f;
				}
			}
			theme.setVolume(volume);
			fading.setFillColor(Color(0,0,0,transparency));
			draw();
	}
	
	return;

}
//Tehd‰‰‰n makee feidaus kun menuun siirryt‰‰n
void Menu::fadeIn()
{
	Clock clock;
	transparency = 255;

	while (transparency >= 0)
	{
		accumulator += clock.getElapsedTime().asSeconds();
			clock.restart();

			while (accumulator >= 0.0005f && transparency >= 0)
			{
				accumulator -= 0.0005f;
				transparency-= 0.1f;
			}
			fading.setFillColor(Color(0,0,0,transparency));
			draw();
	}
	return;

}

//handlataan menun tapahtumia, eli p‰‰asiassa nappien painamisia.
int Menu::processMenuEvents()
{
	Event event;
		while(rw->pollEvent(event))
		{

			if (event.type == Event::Closed)
			{
				return -1;
			}

			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Up)
				{
					
					if (menuselection > 0)
					{
						menuselection--;
					}
					adjustButtons();
				}


				if (event.key.code == Keyboard::W)
				{
					
					if (menuselection > 0)
					{
						menuselection--;
					}
					adjustButtons();
				}

				if (event.key.code == Keyboard::S)
				{
					if (menuselection < 3)
					{
						menuselection++;
					}
					adjustButtons();
				}

				if (event.key.code == Keyboard::Down)
				{
					if (menuselection < 3)
					{
						menuselection++;
					}
					adjustButtons();
				}


				if (event.key.code == Keyboard::Return)
				{
					switch (menuselection)
					{
						case 0:
							//fadeOut();
							return 1;
							break;
						case 1:
							//Custom game koodi
							cout << "custom valittu";
							return 2;
							break;
						case 2:
							system("explorer http://student.labranet.jamk.fi/~G7902/softwareproject_networking/softwareproject_scoresystem/load.php"); //avataan oletusselain high score-sivulle
							break;
						case 3:
							return -1;
							break;
					}


				}
			}

			if(event.type == Event::MouseMoved) //valitaan nappula jos pelaaja hoveraa hiiren osoitinta sen p‰‰ll‰
			{
				if (newButton.getGlobalBounds().contains(Vector2f(rw->mapPixelToCoords(Mouse::getPosition(*rw), rw->getView()).x, rw->mapPixelToCoords(Mouse::getPosition(*rw), rw->getView()).y)))
				{
					menuselection = 0;
					adjustButtons();
				}

				if (customButton.getGlobalBounds().contains(Vector2f(rw->mapPixelToCoords(Mouse::getPosition(*rw), rw->getView()).x, rw->mapPixelToCoords(Mouse::getPosition(*rw), rw->getView()).y)))
				{
					menuselection = 1;
					adjustButtons();
				}

				if(scoreButton.getGlobalBounds().contains(Vector2f(rw->mapPixelToCoords(Mouse::getPosition(*rw), rw->getView()).x, rw->mapPixelToCoords(Mouse::getPosition(*rw), rw->getView()).y)))
				{
					menuselection = 2;
					adjustButtons();
				}

				//if (event.mouseButton.x >= quitButton.getPosition().x && event.mouseButton.x <= (quitButton.getPosition().x + newButton.getGlobalBounds().width) && event.mouseButton.y >= quitButton.getPosition().y && event.mouseButton.y <= (quitButton.getPosition().y + newButton.getGlobalBounds().height) )
				if(quitButton.getGlobalBounds().contains(Vector2f(rw->mapPixelToCoords(Mouse::getPosition(*rw), rw->getView()).x, rw->mapPixelToCoords(Mouse::getPosition(*rw), rw->getView()).y)))
				{
					menuselection = 3;
					adjustButtons();
				}
			}

			if (event.type == Event::MouseButtonPressed) //jos pelaaja klikkaa, tarkastetaan onko pelaaja osunnut nappulaan, ja suoritetaan asianmukaiset toiminnot mik‰li n‰in on.
			{
				SetFocus(rw->getSystemHandle());
				//if (event.mouseButton.x >= newButton.getPosition().x && event.mouseButton.x <= (newButton.getPosition().x + newButton.getGlobalBounds().width) && event.mouseButton.y >= newButton.getPosition().y && event.mouseButton.y <= (newButton.getPosition().y + newButton.getGlobalBounds().height) )
				if (newButton.getGlobalBounds().contains(Vector2f(rw->mapPixelToCoords(Mouse::getPosition(*rw), rw->getView()).x, rw->mapPixelToCoords(Mouse::getPosition(*rw), rw->getView()).y)))
				{
					menuselection = 0;
					adjustButtons();
					Score::setIsSent(false);
					return 1;
				}

				if(customButton.getGlobalBounds().contains(Vector2f(rw->mapPixelToCoords(Mouse::getPosition(*rw), rw->getView()).x, rw->mapPixelToCoords(Mouse::getPosition(*rw), rw->getView()).y)))
				{
					menuselection = 1;
					adjustButtons();
					Score::setIsSent(false);
					return 2;
				}

				if(scoreButton.getGlobalBounds().contains(Vector2f(rw->mapPixelToCoords(Mouse::getPosition(*rw), rw->getView()).x, rw->mapPixelToCoords(Mouse::getPosition(*rw), rw->getView()).y)))
				{
					menuselection = 2;
					adjustButtons();
					Score::setIsSent(false);
					system("explorer http://student.labranet.jamk.fi/~G7902/softwareproject_networking/softwareproject_scoresystem/load.php");
				}

				//if (event.mouseButton.x >= quitButton.getPosition().x && event.mouseButton.x <= (quitButton.getPosition().x + newButton.getGlobalBounds().width) && event.mouseButton.y >= quitButton.getPosition().y && event.mouseButton.y <= (quitButton.getPosition().y + newButton.getGlobalBounds().height) )
				if(quitButton.getGlobalBounds().contains(Vector2f(rw->mapPixelToCoords(Mouse::getPosition(*rw), rw->getView()).x, rw->mapPixelToCoords(Mouse::getPosition(*rw), rw->getView()).y)))
				{
					menuselection = 3;
					adjustButtons();
					Score::setIsSent(false);
					return -1;
				}

				if(versionCheckText.getGlobalBounds().contains(Vector2f(rw->mapPixelToCoords(Mouse::getPosition(*rw), rw->getView()).x, rw->mapPixelToCoords(Mouse::getPosition(*rw), rw->getView()).y)))
				{ 
					if(newVersionReleased)
					{
						string fullLink = "explorer " + downloadLink;
						system(fullLink.c_str());
					}
				}
			}

		}

}


void Menu::updateScoreStatus() //tarkistet‰‰n pelaajan l‰hett‰m‰n scoren tila, ja ilmoitetaan vasemmasssa yl‰nurkassa mit‰ score-paketille oikein tapahtui
{
	if(Score::getIsSent())
	{
		code = Score::getSentCode();
	}
	else
	{
		code = DefaultStatus;
	}

	switch(code)
	{
		case DefaultStatus:
			scoreInfoText.setString("");
			break;
		case Success:
			scoreInfoText.setString("Your score has been sent!");
			scoreInfoText.setColor(Color(50, 200, 50, 255));
			break;
		case ServerError:
			scoreInfoText.setString("Server error whilst sending score.");
			scoreInfoText.setColor(Color::Red);
			break;
		case InvalidName:
			scoreInfoText.setString("Problem with player name whilst sending score.");
			scoreInfoText.setColor(Color::Red);
			break;
		case ClientOutdated:
			scoreInfoText.setString("Client outdated, score not sent.");
			scoreInfoText.setColor(Color::Red);
			break;
		case InvalidScore:
			scoreInfoText.setString("Score not accepted");
			scoreInfoText.setColor(Color::Red);
			break;
		default:
			scoreInfoText.setString("");
			break;
	}
}

//piirret‰‰n menun nappulat ja taustat ja random-tekstit. Aika simppeli.
void Menu::draw()
{
        rw->draw(backgroundPic);
		rw->draw(newButton);
		rw->draw(quitButton);
		rw->draw(scoreButton);
		rw->draw(customButton);
		rw->draw(scoreInfoText);
		rw->draw(versionCheckText);
		rw->draw(fading);
		rw->display();
}

//katsotaan mik‰ nappula on valittuna, piirret‰‰n se kirkkaampana kuin muut nappulat
void Menu::adjustButtons()
{
	switch (menuselection)
	{
	case 0:
		newButton.setFillColor(Color(255,255,255));
	    quitButton.setFillColor(Color(175,175,175));
		scoreButton.setFillColor(Color(175,175,175));
		customButton.setFillColor(Color(175,175,175));
		break;
	case 1:
		newButton.setFillColor(Color(175,175,175));
		quitButton.setFillColor(Color(175,175,175));
		scoreButton.setFillColor(Color(175,175,175));
		customButton.setFillColor(Color(255,255,255));
		break;
	case 2:
		newButton.setFillColor(Color(175,175,175));
		quitButton.setFillColor(Color(175,175,175));
		scoreButton.setFillColor(Color(255,255,255));
		customButton.setFillColor(Color(175,175,175));
		break;
	case 3:
		newButton.setFillColor(Color(175,175,175));
		quitButton.setFillColor(Color(255,255,255));
		scoreButton.setFillColor(Color(175,175,175));
		customButton.setFillColor(Color(175,175,175));
		break;
	}
}