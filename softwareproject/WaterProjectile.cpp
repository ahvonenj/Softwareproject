#include "WaterProjectile.h"


WaterProjectile::WaterProjectile(float radius, TextureManager* texturemanager, Vector2f spawnPosition, Vector2f moveDirection, float playerRotation, int graphicsQuality) : 
	CircleShape(radius),
	lifetime(18.f),
	pAccel(1.0f),
	pVel_x(0),
	pVel_y(0),
	pVelMax(6.f),
	radiusMax(7.8f),
	radiusMultiplier(0.95f),
	Entity(texturemanager, graphicsQuality)
{
	this->isAlive = true;
	this->texturemanager = texturemanager;
	this->setTexture(&texturemanager->get(Textures::WaterTexture));
	this->setOrigin(this->getRadius(), this->getRadius());

	int ra = (rand()%(250 - 200))+200;
	int ralpha = (rand()%(200 - 160))+160;
	//int rb = (rand()%(200 - 150))+150;
	this->setFillColor(Color(190, ra, ra, ralpha));

	/*this->shadow.setTexture(texturemanager->get(Textures::WaterTexture));
	this->shadow.setScale((1 / (22 / this->getRadius()) * 10), (1 / (67 / this->getRadius()) * 10));
	this->shadow.setColor(Color(175, 200, 200, 180));
	this->shadow.setOrigin(this->getRadius(), this->getRadius());
	this->shadow.setPosition(this->getPosition().x + 1, this->getPosition().y + 1);
	this->shadow.setRotation(this->getRotation());*/

	this->moveDirection = moveDirection;

	//Käännetään projectilen suunnan yksikkövektoria 90 astetta myötäpäivään, että saadaan projectilen spawnisijaintiin osoittava vektori (tai ainakin apuvektori)
	Vector2f moveDirectionRotated90(-moveDirection.y, moveDirection.x);
	//Asetetaan projectilen spawnisijainti noin about pelaajan spriten vesipyssyn piippuun
	this->setPosition(Vector2f(spawnPosition.x + moveDirection.x * 17 + moveDirectionRotated90.x * 10, spawnPosition.y + moveDirection.y * 17 + moveDirectionRotated90.y * 10));
}

Time projectileUpdateTime;

void WaterProjectile::update(Time time)
{
	projectileUpdateTime += time;

	if(isAlive)
	{
		//Tämä tapahtuu 10ms välein
		if(projectileUpdateTime >= milliseconds(10.f))
		{
			lifetime -= 1.f; //elinaika pienenee

			this->setRadius(this->getRadius() + radiusMultiplier); //Radius suurenee

			if(this->getRadius() > radiusMax)
			{
				this->setRadius(radiusMax);
			}
			else
			{
				this->setPosition(this->getPosition().x - radiusMultiplier, this->getPosition().y - radiusMultiplier);
			}
			
			updatePosition();

			projectileUpdateTime -= milliseconds(10.f);
		}

		//updateEntityShadow(this->getPosition(), this->getRotation());

		//Projectile merkataan "kuolleeksi" projectilemanagerin cleanausta sun muuta varten
		if(lifetime <= 0.f)
		{
			this->isAlive = false;
		}	
	}
}

bool WaterProjectile::getAlive()
{
	return this->isAlive;
}

void WaterProjectile::updatePosition()
{
	//Projectilen lento
	pVel_x += pAccel * moveDirection.x * (1 + (float)(rand() % 4)) / 2.6f;
	pVel_y += pAccel * moveDirection.y * (1 + (float)(rand() % 4)) / 2.6f;

	if(this->pVel_x > this->pVelMax)
		this->pVel_x = this->pVelMax;

	if(this->pVel_y > this->pVelMax)
		this->pVel_y = this->pVelMax;

	if(this->pVel_x < -this->pVelMax)
		this->pVel_x = -this->pVelMax;

	if(this->pVel_y < -this->pVelMax)
		this->pVel_y = -this->pVelMax;

	float rx = (rand() % 6)*moveDirection.x;
	float ry = (rand() % 6)*moveDirection.y;
	this->setPosition(Vector2f(this->getPosition().x + pVel_x + rx, this->getPosition().y + pVel_y + ry));
			

	if(this->getPosition().x + this->getRadius() < 0 || this->getPosition().x + this->getRadius() > 800 || this->getPosition().y + this->getRadius() < 0 || this->getPosition().y + this->getRadius() > 800)
	{
		this->isAlive = false;
	}
}

WaterProjectile::~WaterProjectile()
{
}
