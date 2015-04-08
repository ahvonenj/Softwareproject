#include "Game.h"
#include "Menu.h"

using namespace std;
using namespace sf;

const Time Game::TimePerFrame = seconds(1.f/60.f);

Game::Game(char argv, RenderWindow *renderwindow, string versionHash, char playercnt) : 
	rw(renderwindow),
	fpsUpdateTime(),
	fpsUpdateFrames(),
	fpsText(),
	isRunning(true),
	rotDir(0),
	endTimeSet(false),
	shakeAmount(140),
	lockMouse(false),
	isPaused(false),
	hitEffectPending(false),
	scoreBeenSet(false),
	isMultiplayer(false),
	version("8_12_13_1"),
	rectSet(false),
	versionHash(versionHash),
	inireader("Resources/settings/customgamesettings.ini"),
	parallaxAmount(2),
	gameType(1),
	timePaused(microseconds(0))
{
	//argv = 'L';

	//Asetetaan grafiikoiden laatu
	if(argv == 'L')
	{
		graphicsQuality = 0;
	}
	else if(argv == 'H')
	{
		graphicsQuality = 1;
	}
	else
	{
		graphicsQuality = 1;
	}

	//Asetetaan multiplayer on/off
	if(playercnt == '1')
	{
		isMultiplayer = false;
	}
	else
	{
		isMultiplayer = true;
	}

	cout << "Version: " << version << endl;

	tm = new TextureManager();
	sm = new SfxManager();

	loadResourcesToMemory();
	setupGame();
	score.setVersionHash(versionHash);

}

int Game::Run() //pelin p‰‰looppi
{
	if(gameType == 0)
	{
		customSettings();
	}
	else
	{
		defaultSettings();
	}


	checkMultiplayer(); //tarkistetaan onko multiplayer, luodaan musat tulille ja menoks
	music.play();
	Clock clock;
    Time timeSinceLastUpdate = Time::Zero;
	totalElapsedTime = new Clock();

    while (isRunning)
    {
		if (processEvents() == false)
		{
			Restart(); //jos processEvents palauttaa false -> pelaaja on p‰‰tt‰nyt pelin joko kuolemalla, pause-menun kautta tai sulkemalla ikkunan. Sitten palataan takaisin p‰‰looppiin
			music.stop();
			return 0;
		}

		timeSinceLastUpdate += clock.restart();

		while (timeSinceLastUpdate > TimePerFrame) //Meid‰n t‰ytyy rajoittaa pelin logiikan nopeutta ottamalla delta-aika. N‰in peli ei toimi eri koneilla maksiminopeudella.
		{
			timeSinceLastUpdate -= TimePerFrame;		

			if (processEvents() == false)
			{
			Restart();
			music.stop();
			return 0;
			}

			if(!isPaused)
			{
				update(TimePerFrame); //jos peli ei ole pausella, p‰ivitet‰‰n pelin logiikkaa. 
				handleInput();
			}
			if (isPaused)
			{	
				totalElapsedTime->restart(); //koska SFML:n kelloa ei pysty pys‰ytt‰m‰‰n, vain resettaamaan, joudumme resettaamaan sen kun peli on pausella. Muuten pelaaja pystyisi ker‰‰m‰‰n ikuisesti scorea pit‰m‰ll‰ peli‰ pausella
			}
		}			

		render(); //lopuksi hoidetaan renderˆinti, piirret‰‰n renderwindowille kaikki pelin tapahtumat	
		updateFps(TimePerFrame);
    }
	return 0; //t‰h‰n kohti ei pit‰isi koskaan p‰‰st‰, palautetaan varmuudeksi 0.
}

//Eventit (L‰hinn‰ ikkunan sulkeminen)
bool Game::processEvents()
{
	Event event;

    while (rw->pollEvent(event))
    {
        if (event.type == Event::Closed)
		{
            return false;
		}

		if(event.type == Event::MouseButtonPressed)
		{
			SetFocus(rw->getSystemHandle());
		}

		if(event.type == Event::Resized) // TODO: Ei toimi
		{
			sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
			rw->setView(View(visibleArea));
		}
   
		if(!isPaused)
		{
			if(playerVec[0]->getAlive())
			{
				if(Keyboard::isKeyPressed(Keyboard::Escape))
				{
					//sf::clockii ei voi pys‰ytt‰‰, pit‰‰ otta pauseTime ylˆs ja restartata clockia kun on pausella. Kiitti Laurent.
					timePaused += totalElapsedTime->getElapsedTime();
					isPaused = true;
					music.pause();
				}
			}
			else
			{
				if(Keyboard::isKeyPressed(Keyboard::Escape))
				{
					return false;
				}			
			}
		}
			else
			{
				if(Mouse::isButtonPressed(Mouse::Button::Left))
				{
					isPaused = false; //jos klikataan hiirenoikeata kun pausella -> peli jatkuu
					music.play();
				}
				if(Keyboard::isKeyPressed(Keyboard::Q)) //jos painetaan Q kun pausella -> peli p‰‰ttyy
					return false;
			}


		if (!playerVec[0]->getAlive() && !playerVec[1]->getAlive()) //jos molemmat pelaajat ovat kuolleet, peli p‰‰ttyy
		{
			if(event.type == Event::KeyPressed) //jos pelaaja on kuollut, otetaan pelaajalta inputti joka tallennetaan scoren nimikentt‰‰n
			 {
				if(event.key.code == Keyboard::BackSpace)
				{
					if (playerInputString.getSize() != 0) // varmistetaan ettei pelaaja voi poistaa enemm‰n kuin on stringiss‰ tilaa. 
					playerInputString.erase(playerInputString.getSize() - 1);

				}

				if (event.key.code == Keyboard::Return)
				{
					if (!isMultiplayer && gameType == 1) { //jos ei ole kyseess‰ moninpeli, eik‰ customgame, niin l‰hetet‰‰n score ja pelaajan nimi serverille, ja peli p‰‰ttyy
						score.setPlayerName(playerInputString);
						score.sendScore();
						score.setIsSent(true);
						}
					return false;
				}

				}
		
			else if (event.type == Event::TextEntered)
			{
				if (event.text.unicode != 8 && playerInputString.getSize() <= 20) // 8 on Backspacen unicode-symboli, n‰in BackSpacea ei voida tulkita merkkin‰ jota halutaan kirjoittaa nimeen
				{ 
				playerInputString.insert(playerInputString.getSize(), event.text.unicode);
				}
			}
		}
	}

	return true; 
}

void Game::update(Time time)
{
	if(graphicsQuality == 1)
		shader.setParameter("time", shaderclock.getElapsedTime().asSeconds()); //jos peli‰ pelataan korkean grafiikan asetuksilla, p‰ivitet‰‰n shaderin aika-muuttujaa.

	if (!isMultiplayer)
		view.setCenter(playerVec[0]->getPosition()); //jos yksinpeli, niin kamera seuraa kokoajan pelaajaa

	if (isMultiplayer && !playerVec[0]->getAlive())
	{
		view.setCenter(playerVec[1]->getPosition());
	}

	else if (isMultiplayer && !playerVec[1]->getAlive())
	{
		view.setCenter(playerVec[0]->getPosition());
	}

	rw->setView(view);
	//p‰ivitet‰‰n pelin HUD:in tekstej‰
	updateTime();
	updateAmmo();
	updateHealth();
	updateLavaGroundRatio(); 
	updateScore();
	updateTextInput();
	world->update(time); //kutsutaan pelimaailman p‰ivitysmetodia
	updateProjectiles(time); //P‰ivitet‰‰n projectilet
	
	projectilemanager->cleanEntities(); //TƒRKEƒ: Tyhjent‰‰ projectilevektorin kuolleista entityist‰ ettei siell‰ ole miljoonia
	updateMusic();

	lastPos = playerVec[0]->getPosition(); //tarvitaan parallax-scrollingiin
	updatePlayer(time); //kutsutaan pelaajan update-metodia

	applyParallax();

	//Mik‰li ei haluta ett‰ powerupit spawnaa heti kun peli alkaa
	if(totalElapsedTime->getElapsedTime().asSeconds() > 25 && !world->isSpawnEnabled())
	{
		world->enableSpawn();
	}
}

void Game::render()
{
	rw->clear(); //pyyhit‰‰n ikkuna mustalla
	drawBackground();
	world->draw();
	drawEntities();

	if(playerVec[0]->getAlive()) //piirret‰‰n pelaaja No. 1 jos se on elossa, jos high graphics niin piirret‰‰n myˆs pelaajan varo.
	{
		if(graphicsQuality == 1)
			rw->draw(playerVec[0]->getShadow());
		rw->draw(*playerVec[0]);	
	}

	if(playerVec[1]->getAlive() && isMultiplayer) //sama kuin ‰sken
	{
		if(graphicsQuality == 1)
			rw->draw(playerVec[1]->getShadow());
		rw->draw(*playerVec[1]);	
	}

	if(hiteffect.getDrawEffect()) //jos pelaaja on ottanut vahinkoa ja hitEffect on elossa, piirret‰‰n efekti ruudulle.
	{
		rw->draw(hiteffect);
	}

	drawTexts();

	rw->display(); //peli-ikkunaan piirret‰‰n bufferiin piirretyt asiat.
}


Clock fpsClock;
float lastTime = 0;

void Game::updateFps(Time time)
{
	fpsUpdateTime += time;
	float currentTime = fpsClock.restart().asSeconds();
	float fps = 1.f / currentTime;
	lastTime = currentTime;

	if(fpsUpdateTime >= seconds(1.f)) //fps p‰ivittyy sekuntin v‰lein
	{
		if(fps >= 60)
		{
			fpsText.setColor(Color::Green);
		}
		else if(fps < 25)
		{
			fpsText.setColor(Color::Red);
		}
		else if(fps < 60)
		{
			fpsText.setColor(Color::Yellow);
		}

		string fpsString(to_string(fps));
		fpsString = fpsString.substr(0,  fpsString.find_first_of( '.' ));

		fpsText.setString("FPS: " + fpsString + "\n");
		fpsUpdateTime -= seconds(1);
	}
}

FloatRect r;
//P‰ivitt‰‰ aikaa oikeeseen yl‰kulmaan jos pelaaja on elossa, muuten siirt‰‰ ajan keskelle ruutua ja n‰ytt‰‰ kuinka kauan pelaaja selviytyi
void Game::updateTime()
{
	if(!playerVec[0]->getAlive() && !playerVec[1]->getAlive())
	{
		timeText.setColor(Color::White);
		timeText.setCharacterSize(40.f);	
		string timeString(to_string(endTime));

		timeString = timeString.substr(0,  timeString.find_first_of( '.' ) + 3);

		timeText.setString("Survival time: " + timeString + "s");

		//V‰h‰n rikki. Ei keskit‰ jos resizee ruudun
		timeText.setPosition(rw->getSize().x / 2 - timeText.getGlobalBounds().width / 2, rw->getSize().y / 2 - timeText.getGlobalBounds().height / 2);
	}
	else
	{
		timeText.setString("Time: " + to_string((totalElapsedTime->getElapsedTime().asMilliseconds() + timePaused.asMilliseconds()) / 1000) + " sec");
	}
}

// Jos pelaaja on kuollut, piidet‰‰n ruudulle teksti jossa player n‰kee kirjoittamansa nimen
void Game::updateTextInput()
{
	if (!isMultiplayer) { //vain single-playerissa voi syˆtt‰‰ nimens‰
		if(!playerVec[0]->getAlive())
		{
			playerInputText.setString("Name: " + playerInputString);
			playerInputText.setPosition(rw->getSize().x / 2 - timeText.getGlobalBounds().width / 2, rw->getSize().y / 2 + timeText.getGlobalBounds().height / 2 + 100);
		}
	}
}

//Joitain inputtien hoitoja
void Game::handleInput()
{
	//Saa pelaajan katsomaan hiirt‰ kohti/tatin osoittamaan suuntaan
	playerVec[0]->faceMouse();
	playerVec[1]->faceMouse();

	//Jos lockmouse-option on valittu niin forcettaa hiiren ruudun sis‰‰n
	if(lockMouse)
	{
		if(Mouse::getPosition(*rw).x < 5)
			Mouse::setPosition(Vector2i(5, Mouse::getPosition(*rw).y), *rw);

		if(Mouse::getPosition(*rw).x > rw->getSize().x - 5)
			Mouse::setPosition(Vector2i(rw->getSize().x - 5, Mouse::getPosition(*rw).y), *rw);

		if(Mouse::getPosition(*rw).y < 5)
			Mouse::setPosition(Vector2i(Mouse::getPosition(*rw).x, 5), *rw);

		if(Mouse::getPosition(*rw).y > rw->getSize().y - 5)
			Mouse::setPosition(Vector2i(Mouse::getPosition(*rw).x, rw->getSize().y - 5), *rw);
	}
}

float ratio;
//p‰ivitet‰‰n laavan suhdetta kiveen
void Game::updateLavaGroundRatio()
{
	ratio = ((float)world->getCurrentLavaTiles() / ((float)world->getCurrentLavaTiles() + (float)world->getCurrentGroundTiles())) * 100; 

	string lavaText(to_string(ratio));
	lavaText = lavaText.substr(0,  lavaText.find_first_of( '.' ) + 2);

	lavaGroundRatioText.setString("Lava-%: " + lavaText + " %");
}

void Game::updateAmmo()
{
	ammoText.setString("Watergun: " + to_string(playerVec[0]->getAmmo()) + " bar");
	if (isMultiplayer)
		p2ammoText.setString("Watergun: " + to_string(playerVec[1]->getAmmo()) + " bar");
}

void Game::updateHealth()
{
	healthText.setString("Health: " + to_string(playerVec[0]->getHealth()) + " %");
	if (isMultiplayer)
		p2healthText.setString("Health: " + to_string(playerVec[1]->getHealth()) + " %");
}

//p‰ivitt‰‰ scorea yl‰nurkkaan. Jos pelaaja on kuollut, lasketaan viimeinen score
void Game::updateScore()
{
	if(!playerVec[0]->getAlive() && !playerVec[1]->getAlive())
	{
		ScoreText.setColor(Color::White);
		ScoreText.setCharacterSize(40.f);

		if (scoreBeenSet == false)
		{
			score.setScore(score.getScore() + (100 * endTime)); //ajasta saa bonuskertoimen scoreen.
			scoreBeenSet = true;
		}

		string scoreString(to_string(score.getScore()));

		ScoreText.setString("Final score: " + scoreString + " points");
		ScoreText.setPosition(rw->getSize().x / 2 - timeText.getGlobalBounds().width / 2, rw->getSize().y / 2 + timeText.getGlobalBounds().height / 2 + 50);
	}
	else
	{
		ScoreText.setString("Score: " + to_string(score.getScore()));
	}
}

// Ebin parallax, eli tausta liikkuu eri tahdissa pelikentt‰‰n, eli hieno efekti
void Game::applyParallax()
{
	backgroundB->setPosition(backgroundB->getPosition().x + ((playerVec[0]->getPosition().x - lastPos.x) / parallaxAmount), backgroundB->getPosition().y + ((playerVec[0]->getPosition().y - lastPos.y) / parallaxAmount));
	//stageShadow->setPosition(stageShadow->getPosition().x + ((playa->getPosition().x - lastPos.x) / 8), stageShadow->getPosition().y + ((playa->getPosition().y - lastPos.y)) / 8);
}

//p‰ivitell‰‰n pelaajaa
void Game::updatePlayer(Time time)
{
	if(playerVec[1]->getAlive())
	{
		playerVec[1]->update(time);
	}

	if(playerVec[0]->getAlive())
	{
		playerVec[0]->update(time);
	}

	else if (!playerVec[0]->getAlive() && !playerVec[1]->getAlive())
	{
		if(!endTimeSet)
		{
			playerVec[0]->setColor(Color(255, 255, 255, 0));
			playerVec[1]->setColor(Color(255,255,255,0));

			endTime = totalElapsedTime->getElapsedTime().asSeconds() + timePaused.asSeconds();
			endTimeSet = true;
		}
	}
	
	manageGUIHeads(); 

	//Punaisen n‰ytˆn v‰l‰ytt‰miseen kun otetaan osumaa
	if(playerVec[0]->getIsHit())
	{
		hiteffect.setDrawEffect(true);
		playerVec[0]->setIsHit(false);
	}

	else if (playerVec[1]->getIsHit())
	{
		hiteffect.setDrawEffect(true);
		playerVec[1]->setIsHit(false);
	}
	
	else
	{
		hiteffect.setDrawEffect(false);
		hiteffect.hide(time);
	}
}

//Siistit‰‰n projektiilej‰
void Game::updateProjectiles(Time time)
{
	if(projectilemanager->getEntityList()->size() != 0)
	{
		for(int i = 0; i < projectilemanager->getEntityList()->size(); i++)
		{
			projectilemanager->getEntityList()->at(i)->update(time);
		}
	}
}
//Musiikin pitch kasvaa mit‰ pidemp‰‰ pelaaja on selvinnyt
void Game::updateMusic()
{
	music.setPitch(music.getPitch() + 0.00001f);
	
	if(music.getPitch() > 1.3f)
		music.setPitch(1.3f);
}

//Alkuefekti ... offilla
void Game::shakeView()
{
	if(shakeAmount > 0)
	{
		if(rotDir == 0)
		{
			view.rotate(-2.0f);
			rotDir = 1;
		}
		else
		{
			view.rotate(2.0f);
			rotDir = 0;
		}
		shakeAmount--;
	}
}

//Piirret‰‰n tausta. jos on korkean grafiikan laatu, niin k‰ytet‰‰n shaderia
void Game::drawBackground()
{
	if(graphicsQuality == 1)
	{
		states.shader = &shader;
		rw->draw(*background, states);
	}
	else
	{
		rw->draw(*backgroundB);
	}
}

//piirret‰‰n pelaajan ammukset
void Game::drawEntities()
{
	if(projectilemanager->getEntityList()->size() != 0)
	{
		for(int i = 0; i < projectilemanager->getEntityList()->size(); i++)
		{
			if(projectilemanager->getEntityList()->at(i)->getAlive())
			{			
				rw->draw(*projectilemanager->getEntityList()->at(i));
				projectilemanager->getEntityList()->at(i)->drawShadow(rw);
			}
		}
	}
}

//Piirt‰‰ GUI ja tekstit
void Game::drawTexts()
{
	rw->setView(rw->getDefaultView());

	rw->draw(fpsText);
	rw->draw(GUIhead);
	rw->draw(timeText);
	rw->draw(ammoText);
	rw->draw(healthText);
	rw->draw(lavaGroundRatioText);
	rw->draw(ScoreText);

	if (isMultiplayer)
	{
		rw->draw(P2GUIhead);
	}

	if (playerVec[1]->getAlive() && isMultiplayer)
	{
		rw->draw(p2ammoText);
		rw->draw(p2healthText);
	}

	if(!playerVec[0]->getAlive() && !isMultiplayer)
		rw->draw(playerInputText);

	if(isPaused)
		rw->draw(pauseText);

	rw->setView(view);
}

//nollataan pelin asetukset
void Game::Restart()
{

	endTimeSet = false;
	music.setPitch(1);
	resetTimeText();
	resetScoreText();
	totalElapsedTime->restart();
	timePaused = microseconds(0);
	playerVec[0]->Reset();
	playerVec[1]->Reset();
	world->Reset();
	score.resetStats();
	playerInputString = "";
	isPaused = false;
	scoreBeenSet = false;
}
//ladataan pelin resurssit managereihin
void Game::loadResourcesToMemory()
{
	this->tm->load(Textures::LavaTexture,"Resources/LavaSprite.png");
	this->tm->load(Textures::GroundTexture,"Resources/GroundSprite.png");
	this->tm->load(Textures::WaterTexture,"Resources/WaterSprite.png");
	this->tm->load(Textures::MieskoHealthy,"Resources/Palomiesko_healthy.png");
	this->tm->load(Textures::MieskoBruised,"Resources/Palomiesko_bruised.png");
	this->tm->load(Textures::MieskoDead,"Resources/Palomiesko_dead.png");
	this->tm->load(Textures::background_a,"Resources/background_a.png");
	this->tm->load(Textures::P2MieskoHealthy,"Resources/Palomiesko_healthy_p2.png");
	this->tm->load(Textures::P2MieskoBruised,"Resources/Palomiesko_bruised_p2.png");
	this->tm->load(Textures::P2MieskoDead,"Resources/Palomiesko_dead_p2.png");
	this->tm->load(Textures::Blank,"Resources/BlankTexture.png");
	this->tm->load(Textures::Pixel,"Resources/pixel.gif");
	this->tm->load(Textures::Droplet,"Resources/Droplet.png");
	this->tm->load(Textures::Player,"Resources/PlayerSprite.png");
	this->tm->load(Textures::Player2,"Resources/P2PlayerSprite.png");

	//Hyvill‰ grafiikoilla asetetaan spritejen setsmooth-optioni p‰‰lle
	if(graphicsQuality == 1)
	{
		this->tm->get(Textures::LavaTexture).setSmooth(true);
		this->tm->get(Textures::GroundTexture).setSmooth(true);
		this->tm->get(Textures::WaterTexture).setSmooth(true);
		this->tm->get(Textures::MieskoHealthy).setSmooth(true);
		this->tm->get(Textures::MieskoBruised).setSmooth(true);
		this->tm->get(Textures::MieskoDead).setSmooth(true);
		this->tm->get(Textures::P2MieskoHealthy).setSmooth(true);
		this->tm->get(Textures::P2MieskoBruised).setSmooth(true);
		this->tm->get(Textures::P2MieskoDead).setSmooth(true);
		this->tm->get(Textures::background_a).setSmooth(true);
		this->tm->get(Textures::Blank).setSmooth(true);
		this->tm->get(Textures::Pixel).setSmooth(true);
		this->tm->get(Textures::Droplet).setSmooth(true);
		//this->tm->get(Textures::Player).setSmooth(true);
	}

	this->sm->load(SoundEffects::Shoot,"Resources/shoot1.wav");
	this->sm->load(SoundEffects::Hit,"Resources/hit3.wav");
	this->sm->load(SoundEffects::Pickup,"Resources/pickup2.wav");
}

//Pitk‰ alustusmetodi. Ladataan fontit, musiikki, tekstit, GUI:n p‰‰t jne...
void Game::setupGame()
{
	font.loadFromFile("Resources/04b.ttf");
	fpsText.setFont(font);
	fpsText.setPosition(5.f, 5.f);
	fpsText.setColor(Color::Black);
	fpsText.setCharacterSize(16.f);
	fpsText.setStyle(Text::Bold);

	timeText.setFont(font);
	timeText.setPosition(5.f, 22.f);
	timeText.setColor(Color::White);
	timeText.setCharacterSize(16.f);
	timeText.setStyle(Text::Bold);
	
	ScoreText.setFont(font);
	ScoreText.setPosition(5.f, 92.f);
	ScoreText.setColor(Color::White);
	ScoreText.setCharacterSize(16.f);
	ScoreText.setStyle(Text::Bold);

	ammoText.setFont(font);
	ammoText.setPosition(5.f, 40.f);
	ammoText.setColor(Color::White);
	ammoText.setCharacterSize(16.f);
	ammoText.setStyle(Text::Bold);

	p2ammoText.setFont(font);
	p2ammoText.setPosition(1100.f, 40.f);
	p2ammoText.setColor(Color::White);
	p2ammoText.setCharacterSize(16.f);
	p2ammoText.setStyle(Text::Bold);

	healthText.setFont(font);
	healthText.setPosition(5.f, 58.f);
	healthText.setColor(Color::White);
	healthText.setCharacterSize(16.f);
	healthText.setStyle(Text::Bold);

	p2healthText.setFont(font);
	p2healthText.setPosition(1100.f, 58.f);
	p2healthText.setColor(Color::White);
	p2healthText.setCharacterSize(16.f);
	p2healthText.setStyle(Text::Bold);

	pauseText.setFont(font);
	pauseText.setString("               PAUSED\n(Mouse left to continue | Q to quit)");
	pauseText.setCharacterSize(16.f);
	pauseText.setStyle(Text::Bold);
	pauseText.setPosition(rw->getSize().x / 2 - pauseText.getGlobalBounds().width / 2, rw->getSize().y / 2 - pauseText.getGlobalBounds().height / 2);
	pauseText.setColor(Color::White);

	lavaGroundRatioText.setFont(font);
	lavaGroundRatioText.setCharacterSize(16.f);
	lavaGroundRatioText.setStyle(Text::Bold);
	lavaGroundRatioText.setPosition(5.f, 75.f);
	lavaGroundRatioText.setColor(Color::White);

	playerInputText.setFont(font);
	playerInputText.setCharacterSize(40.f);
	playerInputText.setStyle(Text::Bold);

	view = rw->getView();

	background_a = tm->get(Textures::Blank);
	background_a.setRepeated(true);

	background = new RectangleShape(Vector2f(3000, 3000));
	background->setTextureRect(IntRect(0, 0, 3000, 3000));
	background->setTexture(&background_a);
	background->setPosition(-1000, -1000);
	background->setFillColor(Color(255, 255, 255, 255));

	background_b = tm->get(Textures::background_a);
	background_b.setRepeated(true);

	backgroundB = new RectangleShape(Vector2f(3000, 3000));
	backgroundB->setTextureRect(IntRect(0, 0, 3000, 3000));
	backgroundB->setTexture(&background_b);
	backgroundB->setPosition(-1000, -1000);
	backgroundB->setFillColor(Color(255, 255, 255, 255));

	stageShadow = new RectangleShape(Vector2f(400, 400));
	stageShadow->setFillColor(Color(0, 0, 0, 100));
	stageShadow->setPosition(400, -200);

	GUIhead.setTexture(tm->get(Textures::MieskoHealthy));
	GUIhead.setScale(0.9f, 0.9f);
	GUIhead.setPosition(20.f, 102.f);

	P2GUIhead.setTexture(tm->get(Textures::P2MieskoHealthy));
	P2GUIhead.setScale(0.9f, 0.9f);
	P2GUIhead.setPosition(1150.f, 102.f);

	if(graphicsQuality == 1)
	{
		if (!shader.loadFromFile("resources/fire.vert", sf::Shader::Fragment)) //ladataan vertex-shaderi
			cout << "Shader ei ladannut!";

		shader.setParameter("resolution", 1920, 1080); // shaderin reso
	}

	projectilemanager = new EntityManager<WaterProjectile*>();
	
	music.openFromFile("Resources/Game_Soundtrack.wav");

	playa = new Player(projectilemanager, tm, rw, sm, graphicsQuality, 1);
	Texture texture = tm->get(Textures::Droplet);
	playa2 = new Player(projectilemanager, tm, rw, sm, graphicsQuality, 2);
	playa2->setAlive(false);

	playerVec.push_back(playa);
	playerVec.push_back(playa2);

	world = new World(rw, *tm, playerVec, projectilemanager, sm, score, graphicsQuality);

	rw->setVerticalSyncEnabled(true);

	music.setVolume(50);
	music.setLoop(true);
	music.setPitch(1.f);

	hiteffect;

	rw->setIcon(40, 40, tm->get(Textures::Player).copyToImage().getPixelsPtr());

}

//Lataa custom settingssit gamelle .ini-tiedostosta
void Game::customSettings()
{
	//Restart();
	cout << endl << "Following custom settings loaded:\n" << endl;

	lockMouse = inireader.ReadBoolean("game", "g_lockmouse", false);
	parallaxAmount = inireader.ReadInteger("game", "g_parallax", 2);
	graphicsQuality = inireader.ReadInteger("game", "g_graphicsquality", 1);

	cout << "g_lockmouse = " << lockMouse << endl;
	cout << "g_parallax = " << parallaxAmount << endl;
	cout << "g_graphicsquality = " << graphicsQuality << endl;

	world->customSettings(&inireader);
	playa->customSettings(&inireader);
	playa2->customSettings(&inireader);
}

//Lataa normaalit asetukset
void Game::defaultSettings()
{
	cout << endl << "Default settings loaded for:\n" << endl;

	cout << "- Game" << endl;

	lockMouse = false;
	parallaxAmount = 2;

	world->defaultSettings();
	playa->defaultSettings();
	playa2->defaultSettings();

	//graphicsQuality = 1;
}

//nollataan aikateksti
void Game::resetTimeText()
{
	timeText.setFont(font);
	timeText.setPosition(5.f, 22.f);
	timeText.setColor(Color::White);
	timeText.setCharacterSize(16.f);
	timeText.setStyle(Text::Bold);

}
//nollataan scoreteksti
void Game::resetScoreText()
{
	ScoreText.setFont(font);
	ScoreText.setPosition(5.f, 92.f);
	ScoreText.setColor(Color::White);
	ScoreText.setCharacterSize(16.f);
	ScoreText.setStyle(Text::Bold);
	scoreBeenSet = false;

}
//tarkistetaan onko moninpeli
void Game::checkMultiplayer()
{
	Joystick::update();
	if (isMultiplayer)
	{
		playerVec[1]->setAlive(true);
		view = rw->getDefaultView();
		view.setCenter(400, 400);
		view.zoom(1.f);
	}
	else
	{
		playerVec[1]->setAlive(false);
		view = rw->getDefaultView();
		view.zoom(0.63f);
	}
}
//p‰ivitell‰‰n mieskolaisen naamaa pelin hudissa
void Game::manageGUIHeads()
{
	if (playerVec[0]->getHealth() > 25 && playerVec[0]->getHealth() < 50)
	{
		GUIhead.setTexture(tm->get(Textures::MieskoBruised));
	}
	else if (playerVec[0]->getHealth() == 0)
	{
		GUIhead.setTexture(tm->get(Textures::MieskoDead));
	}
	else if(playerVec[0]->getHealth() > 50)
	{
		GUIhead.setTexture(tm->get(Textures::MieskoHealthy));
	}

	if (isMultiplayer)
	{
	if (playerVec[1]->getHealth() > 25 && playerVec[1]->getHealth() < 50)
	{
		P2GUIhead.setTexture(tm->get(Textures::P2MieskoBruised));
	}
	else if (playerVec[1]->getHealth() == 0)
	{
		P2GUIhead.setTexture(tm->get(Textures::P2MieskoDead));
	}
	else if(playerVec[1]->getHealth() > 50)
	{
		P2GUIhead.setTexture(tm->get(Textures::P2MieskoHealthy));
	}
	}
}

void Game::setGametype(int gameType)
{
	this->gameType = gameType;
}

Game::~Game()
{
	delete background;
	delete backgroundB;
	delete stageShadow;
	delete projectilemanager;
	delete playa;
	delete world;
	delete tm;
	delete sm;

	background = nullptr;
	backgroundB = nullptr;
	stageShadow = nullptr;
	projectilemanager = nullptr;
	playa = nullptr;
    world = nullptr;
	tm = nullptr;
	sm = nullptr;
}
