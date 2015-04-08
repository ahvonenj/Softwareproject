#include "Player.h"

using namespace sf;
using namespace std;

Player::Player(EntityManager<WaterProjectile*>* projectilemanager, TextureManager* tm, RenderWindow* rw, SfxManager* sm, int graphicsQuality, int playerid) : 
	pAccel(0.40f),
	pVel_x(0),
	pVel_y(0),
	pVelMax(5.f),
	deccelerationMultiplier(1.5f),
	isAlive(true),
	fireSpeed(45.f),
	watergunAmmo(500),
	godMode(false),
	pHealth(100),
	pHealthMax(100),
	drawDist(Rect<float>(0, 0, 1100, 800)),
	isHit(false),
	playerid(playerid),
	canShoot(true)
{

	this->projectilemanager = projectilemanager;
	this->tm = tm;
	this->sm = sm;
	this->rw = rw;
	this->graphicsQuality = graphicsQuality;

	//texture = tm->get(Textures::Player);
	//this->setTexture(texture);
	if (playerid == 1)
	{
	this->setTexture(this->tm->get(Textures::Player)); //annetaa pelaajan teksureksi punahattunen miesko jos pelaajaid on 1, sinihattuinen miesko jos 2
	}

	else if(playerid == 2)
	{
		this->setTexture(this->tm->get(Textures::Player2));
	}
	this->setOrigin(this->getTexture()->getSize().x / 2, this->getTexture()->getSize().y / 2); //Ei kannata muuttaa, asettaa pelaajan paikallisen origon keskelle

	this->setPosition(400, 400);

	this->shadow.setTexture(this->tm->get(Textures::Player));
	this->shadow.setColor(Color(30, 30, 30, 100));
	this->shadow.setOrigin(this->getTexture()->getSize().x / 2, this->getTexture()->getSize().y / 2);
	this->shadow.setPosition(this->getPosition().x + 1, this->getPosition().y + 1);
	this->shadow.setRotation(this->getRotation());
	

}
//CustomGamen asetukset pelaajalle .ini-tiedostosta
void Player::customSettings(IniReader* inireader)
{
	this->pAccel = inireader->ReadFloat("player", "p_acceleration", 0.40f);
	this->pVelMax = inireader->ReadFloat("player", "p_velmax", 5.0f);
	this->deccelerationMultiplier = inireader->ReadFloat("player", "p_decceleration", 1.5f);
	this->fireSpeed = inireader->ReadFloat("player", "p_firespeed", 45.0f);
	this->watergunAmmo = inireader->ReadInteger("player", "p_watergunammo", 500);
	this->godMode = inireader->ReadBoolean("player", "p_godmode", false);
	this->pHealth = inireader->ReadInteger("player", "p_health", 100);
	this->pHealthMax = inireader->ReadInteger("player", "p_healthmax", 100);

	cout << "player " << playerid << " p_acceleration = " << pAccel << endl;
	cout << "player " << playerid << " p_velmax = " << pVelMax << endl;
	cout << "player " << playerid << " p_decceleration = " << deccelerationMultiplier << endl;
	cout << "player " << playerid << " p_firespeed = " << fireSpeed << endl;
	cout << "player " << playerid << " p_watergunammo = " <<watergunAmmo << endl;
	cout << "player " << playerid << " p_godmode = " << godMode << endl;
	cout << "player " << playerid << " p_health = " << pHealth << endl;
	cout << "player " << playerid << " p_healthmax = " << pHealthMax << endl;
	
}
//Pelaajan default-asetukset
void Player::defaultSettings()
{
	cout << "- Player " << playerid << endl;

	pAccel = 0.4f;
	pVelMax = 5.0f;
	deccelerationMultiplier = 1.5f;
	fireSpeed = 45.f;
	watergunAmmo = 500;
	godMode = false;
	pHealth = 100;
	pHealthMax = 100;
}

Vector2f Player::getSpeed()
{
	return Vector2f(pVel_x, pVel_y);
}




//Päivittää pääasiassa pelaajan sijaintia
void Player::update(Time time)
{
	if(isAlive)
	{
		calculateVelocity();
		if (playerid == 2)
		{
		}

		//Vasen hiiren nappin bind & ampuminen
		if(Mouse::isButtonPressed(Mouse::Button::Left) && playerid == 1)
		{
			if(canShoot)
			{
				if(watergunAmmo > 0)
				{
					this->shoot();
					canShoot = false;
				}
			}
		}

		//Tällä säädetään ampumisnopeutta
		playerUpdateTime += time;
		if(playerUpdateTime >= milliseconds(fireSpeed))
		{
			canShoot = true;
			playerUpdateTime -= milliseconds(fireSpeed);
		}

		isPlayerOutOfBounds();
		
		this->p_realtimePosition = this->getPosition();

		updateShadow();
	}

	//Healthin regen 1/s
	healthRegenTime += time;
	if(healthRegenTime >= seconds(1))
	{
		pHealth += 1;
		if(pHealth > pHealthMax)
			pHealth = pHealthMax;

		healthRegenTime -= seconds(1);
	}

}
//päivitetään pelaajan varjoo
void Player::updateShadow()
{
	this->shadow.setPosition(this->getPosition().x + 4, this->getPosition().y + 4);
	this->shadow.setRotation(this->getRotation());
}


bool Player::getAlive()
{
	return this->isAlive;
}

void Player::setAlive(bool alive)
{
	this->isAlive = alive;
}


Vector2f passToProjectile;
void Player::shoot()
{
	if(canShoot)
	{
		if(watergunAmmo > 0)
		{
			joystickR = sf::Joystick::getAxisPosition(0, sf::Joystick::R);
			joystickU = sf::Joystick::getAxisPosition(0, sf::Joystick::U);

			//Lasketaan missä suunnassa hiiri on pelaajaan nähden/tatin kulma ja passataan kyseinen suunta projectilelle, että se osaa lentää oikeeseen suuntaan
			if (playerid == 1)
			{
				passToProjectile.x = (rw->mapPixelToCoords(Mouse::getPosition(*rw), rw->getView()).x - this->getPosition().x) / lenghtOfVectors(rw->mapPixelToCoords(Mouse::getPosition(*rw), rw->getView()), this->getPosition());
				passToProjectile.y = (rw->mapPixelToCoords(Mouse::getPosition(*rw), rw->getView()).y - this->getPosition().y) / lenghtOfVectors(rw->mapPixelToCoords(Mouse::getPosition(*rw), rw->getView()), this->getPosition());
				projectilemanager->addEntity(new WaterProjectile(1.f, tm, this->getPosition(), passToProjectile, this->getRotation(), graphicsQuality)); //Tehdään projectile ja annetaan se projectilemanagerille
		
			}

			else if (playerid == 2)
			{
				if (joystickR > 25 || joystickU > 25 || joystickR < -25 || joystickU < -25) //kannattaa olla vähän thresholdia, joystickin tatit on tosi herkkiä
				{
					float angle = (atan2(joystickR,joystickU)); //saadaan oikeasta tatista x ja y (nimettyinä SFML:ssä R ja U akseleiksi, ja saadaan arctanilla niiden välinen kulma. perus trigonometriapaskaa
					passToProjectile.x = (cos(angle));
					passToProjectile.y = (sin(angle));
					projectilemanager->addEntity(new WaterProjectile(1.f, tm, this->getPosition(), passToProjectile, this->getRotation(), graphicsQuality));
				}
			}

			if(pSfx.getStatus() == Sound::Status::Stopped)
			{
				pSfx.setBuffer(sm->get(SoundEffects::Shoot));
				pSfx.play();
			}

	
			if(!godMode)
				watergunAmmo--;

			canShoot = false;
		}
	}
}

//pelaaja katsoo hiiren suuntaan/tatin osoittamaan suuntaant.
void Player::faceMouse()
{
	if (playerid == 1)
	{
	this->setRotation(AngleBetweenVectors(rw->mapPixelToCoords(Mouse::getPosition(*rw), rw->getView()), this->getPosition()) - 90.f);
	//cout << AngleBetweenVectors(rw->mapPixelToCoords(Mouse::getPosition(*rw), rw->getView()), this->getPosition()) - 90.f << endl;
	}

	if (playerid == 2)
	{
		joystickR = sf::Joystick::getAxisPosition(0, sf::Joystick::R);
		joystickU = sf::Joystick::getAxisPosition(0, sf::Joystick::U);

		if (joystickR > 25 || joystickU > 25 || joystickR < -25 || joystickU < -25)
		{
			this->setRotation((atan2(joystickR,joystickU) * 180 / M_PI) + 90); //oikeasta tatista saadaan kulma joka asetetaan pelaajan rotaatioksi. Kerrotaan 180/Pii jotta saadaan asteina, ja lisätään 90 koska muuten 90 astetta jäljessä
			shoot();
		}

		
	}
}

int Player::getAmmo()
{
	return this->watergunAmmo;
}

void Player::setAmmo(int ammo)
{
	this->watergunAmmo = ammo;
}

bool Player::getGodmode()
{
	return this->godMode;
}

int Player::getHealth()
{
	return this->pHealth;
}

void Player::setHealth(int health)
{
	this->pHealth = health;
}


void Player::hit(int amount)
{
	this->pHealth = this->pHealth - amount;

	if(this->pHealth <= 0)
	{
		this->pHealth = 0;
		this->isAlive = false;
	}
	isHit = true;
}

bool Player::getIsHit()
{
	return isHit;
}

void Player::setIsHit(bool isHit)
{
	this->isHit = isHit;
}

float Player::AngleBetweenVectors(Vector2f vX, Vector2f vY)
{
    return (180 / M_PI) * atan2(vY.y - vX.y, vY.x - vX.x);
}

//MagnitudeOfVectorsNormalized, katotaan yksikkövektorit
Vector2f Player::MofVN(Vector2f vX, Vector2f vY)
{
    float dotproduct = vX.x * vY.x + vX.y * vY.y;
    Vector2f direction = Vector2f(max(vY.x, vX.x) - min(vY.x, vX.x), max(vY.y, vX.y) - min(vY.y, vX.y));
    float dirmag = (float)sqrt((direction.x * direction.x) + (direction.y * direction.y));
    return Vector2f(direction.x / dirmag, direction.y / dirmag);
}
//lasketaan vektorien pituudet
float Player::lenghtOfVectors(Vector2f vX, Vector2f vY)
{
	return sqrt(pow(vY.x - vX.x, 2) + pow(vY.y - vX.y, 2));
}

Sprite Player::getShadow()
{
	return this->shadow;
}

FloatRect Player::getDrawDist()
{
	return drawDist;
}

Vector2f Player::getRealtimePosition()
{
	return p_realtimePosition;
}

void Player::isPlayerOutOfBounds()
{
	//Onko pelaaja pelialueen ulkopuolella
	bool outOfBounds = false;

	//Tarkistellaan...
	if(this->getPosition().x + pVel_x - this->getTexture()->getSize().x / 2 < 0)
	{
		this->setPosition(Vector2f(this->getTexture()->getSize().x / 2, this->getPosition().y + pVel_y));
		outOfBounds = true;
	}
	if(this->getPosition().x + pVel_x + this->getTexture()->getSize().x / 2 > 800)
	{
		this->setPosition(Vector2f(800 - this->getTexture()->getSize().x / 2, this->getPosition().y + pVel_y));
		outOfBounds = true;
	}
	if(this->getPosition().y + pVel_y - this->getTexture()->getSize().y / 2 < 0)
	{
		this->setPosition(Vector2f(this->getPosition().x + pVel_x, 0 + this->getTexture()->getSize().x / 2));
		outOfBounds = true;
	}
	if(this->getPosition().y + pVel_y + this->getTexture()->getSize().y / 2 > 800)
	{
		this->setPosition(Vector2f(this->getPosition().x + pVel_x, 800 - this->getTexture()->getSize().y / 2));
		outOfBounds = true;
	}
	
	if(!outOfBounds)
	{
		this->setPosition(Vector2f(this->getPosition().x + pVel_x, this->getPosition().y + pVel_y));
		this->drawDist.left += pVel_x;
		this->drawDist.top += pVel_y;
	}
}

void Player::calculateVelocity()
{
	//Näppäimiä painettu/ tattia vatkattu.
	bool isX = false;
	bool isY = false;

	//Kiihtyvyys
	if(playerid == 1)
	{
		if(Keyboard::isKeyPressed(Keyboard::Key::D)) //Accel oikealle
		{
			isX = true;
			pVel_x += pAccel;

			if(pVel_x > pVelMax)
				pVel_x = pVelMax;
		}
		if (Keyboard::isKeyPressed(Keyboard::Key::A)) //Accel vasemmalle
		{
			isX = true;
			pVel_x -= pAccel;

			if(pVel_x < -pVelMax)
				pVel_x = -pVelMax;
		}
		if(Keyboard::isKeyPressed(Keyboard::Key::S)) //Accel alas
		{
			isY = true;
			pVel_y += pAccel;

			if(pVel_y > pVelMax)
				pVel_y = pVelMax;
		}
		if (Keyboard::isKeyPressed(Keyboard::Key::W)) //Accel oylös
		{
			isY = true;
			pVel_y -= pAccel;

			if(pVel_y < -pVelMax)
				pVel_y = -pVelMax;
		}
	}
	else
	{

		 joystickX = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
		 joystickY = sf::Joystick::getAxisPosition(0, sf::Joystick::Y); //U on oikean tatin x, R on oikean tatin y


		//Tatille asetetaan kaksi eri kiihtymistasoa, jotta pelaaja voi liikkua hitaammin jos tattia kallistaa vähän, ja nopeammin jos tattia kallistaa enemmän
		if (joystickY > 25 && joystickY< 60) //alas
		{
		isY = true;
		pVel_y += pAccel;

		if(pVel_y > pVelMax/2) {
			pVel_y = pVelMax/2;
			}
		}

		else if ( joystickY < -25 &&  joystickY > -60) // ylos
		{
			isY = true;
			pVel_y -= pAccel;
			if(pVel_y < -pVelMax/2) {
				pVel_y = -pVelMax/2;
			}
		}

		if (joystickX > 25 && joystickX < 60) // oikealle
		{
		isX = true;
		pVel_x += pAccel;

			if(pVel_x > pVelMax/2) {
				pVel_x = pVelMax/2;
			}			
		}

		else if (joystickX < -25 && joystickX > -60) // vasemmalle
		{
			isX = true;
			pVel_x -= pAccel;

			if(pVel_x < -pVelMax/2) {
				pVel_x = -pVelMax/2;
			}
		}

		if (joystickY > 60) //alas
		{
		isY = true;
		pVel_y += pAccel;

		if(pVel_y > pVelMax) {
			pVel_y = pVelMax;
			}
		}

		else if ( joystickY < -60) // ylos
		{
			isY = true;
			pVel_y -= pAccel;
			if(pVel_y < -pVelMax) {
				pVel_y = -pVelMax;
			}
		}

		if (joystickX > 60) // oikealle
		{
		isX = true;
		pVel_x += pAccel;

			if(pVel_x > pVelMax) {
				pVel_x = pVelMax;
			}			
		}

		else if (joystickX < -60) // vasemmalle
		{
			isX = true;
			pVel_x -= pAccel;

			if(pVel_x < -pVelMax) {
				pVel_x = -pVelMax;
			}
		}
		 }
	
	//Hidastuvuus
	if(!isX)
	{
		if(pVel_x < 0) // Hidastuvuus ylös tai alas (en muista)
		{
			pVel_x += pAccel / deccelerationMultiplier;
			if(pVel_x >= 0)
				pVel_x = 0.0f;
		}
		else // Hidastuvuus ylös tai alas (en muista)
		{
			pVel_x -= pAccel / deccelerationMultiplier;
			if(pVel_x <= 0)
				pVel_x = 0.0f;
		}
	}
	if(!isY)
	{
		if(pVel_y < 0) // Hidastuvuus vasen tai oikea (en muista)
		{
			pVel_y += pAccel / deccelerationMultiplier;
			if(pVel_y >= 0)
				pVel_y = 0.0f;
		}
		else // Hidastuvuus vasen tai oikea (en muista)
		{
			pVel_y -= pAccel / deccelerationMultiplier;
			if(pVel_y <= 0)
				pVel_y = 0.0f;
		}
	}

	
}

int Player::getPlayerid()
{
	return playerid;
}

void Player::Reset()
{
	this->isAlive = true;
	this->watergunAmmo = 500;
	this->pHealth = pHealthMax;
	this->setPosition(400, 400);
	this->setColor(Color(255,255,255,255));
	this->pVel_x = 0;
	this->pVel_y = 0;
}

Player::~Player()
{
}
