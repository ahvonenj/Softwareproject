#include "World.h"

using namespace std;
using namespace sf;

World::World(RenderWindow* rw, TextureManager& tm, vector<Player*>& playerVec, EntityManager<WaterProjectile*>* projectilemanager,
			 SfxManager* sm, Score& score,int graphicsQuality) :
	lavaSpreadChance(11),
	currentLavaTiles(0),
	currentGroundTiles(0),
	pickupChance(22),
	spawnEnabled(true)
{
	srand(time(NULL));
	this->rw = rw;
	this->tm = &tm;
	this->projectilemanager = projectilemanager;
	this->playerVec = playerVec;
	this->sm = sm;
	this->score = &score;
	this->graphicsQuality = graphicsQuality;

	powerupmanager = new EntityManager<Powerup*>();

	buffer.loadFromFile("Resources/hit3.wav");
	buffer.loadFromFile("Resources/pickup2.wav");

    //Pelin "kartta"
    char tilemap[WIDTH][HEIGHT];
     
	//Luodaan kertta hienolla luupilla (t‰ss‰ oli ennen j‰rkytt‰v‰n iso array t‰ynn‰ arrayta)
    for(int i = 0; i < WIDTH; i++)
    {
            for(int j = 0; j < HEIGHT; j++)
            {
                    if(i == 0 || j == 0 || i == WIDTH-1 || j == HEIGHT-1)
                    {
                            tilemap[i][j] = 'L';
                    }
                    else
                    {
                            tilemap[i][j] = 'G';
                    }
            }
    }

	//Karttaa ei voinut alustaa headerissa joten kopioidaan tuo ylh‰‰ll‰ alustettu kartta headerissa sijaitsevaan charmap2D:hen
	for(int i = 0; i < WIDTH; i++)
	{
		for(int j = 0; j < HEIGHT; j++)
		{
			this->charmap2D[j][i] = tilemap[j][i];
		}
		
	}

	initalizeWorld();
}

//Ladataan worldille customsettingssit
void World::customSettings(IniReader* inireader)
{
	this->lavaSpreadChance = inireader->ReadInteger("world", "w_lavaspreadchance", 11);
	this->pickupChance = inireader->ReadInteger("world", "w_pickupchance", 22);

	cout << "w_lavaspreadchance = " << lavaSpreadChance << endl;
	cout << "w_pickupchance = " << pickupChance << endl;
}

//Ladataan worldille perusasetukset
void World::defaultSettings()
{
	cout << "- World" << endl;

	lavaSpreadChance = 11;
	pickupChance = 22;
}

//Kartanp‰ivitysmetodi
void World::update(Time time)
{
	currentGroundTiles = 0;
	currentLavaTiles = 0;
	worldUpdateTime += time;

	Vector2f plpos = playerVec[0]->getRealtimePosition();
	Vector2i plposPinpoint = Vector2i((int)floor((plpos.x)/25), (int)floor(plpos.y/25));

	if(plposPinpoint.x < 0)
		plposPinpoint.x = 0;
	if(plposPinpoint.y < 0)
		plposPinpoint.y = 0;
	if(plposPinpoint.x > 31)
		plposPinpoint.x = 31;
	if(plposPinpoint.y > 31)
		plposPinpoint.y = 31;

	//Hirveet‰ haxaysta, jotta tiedet‰‰n t‰sm‰lleen miss‰ tilemapin alkiossa pelaaja periaatteessa on
	if(playerPinpointIntersects(plposPinpoint))
	{
		if(!playerVec[0]->getGodmode())
		{
			if(playerVec[0]->getAlive())
			{
				playerVec[0]->hit(2);

				if(sfx.getStatus() == Sound::Status::Stopped || sfx.getBuffer() == &sm->get(SoundEffects::Pickup))
				{
					sfx.setBuffer(sm->get(SoundEffects::Hit));
					sfx.play();
				}
			}
		}
	}

	Vector2f plpos2 = playerVec[1]->getRealtimePosition();
	Vector2i plposPinpoint2 = Vector2i((int)floor((plpos2.x)/25), (int)floor(plpos2.y/25));

	if(plposPinpoint2.x < 0)
		plposPinpoint2.x = 0;
	if(plposPinpoint2.y < 0)
		plposPinpoint2.y = 0;
	if(plposPinpoint2.x > 31)
		plposPinpoint2.x = 31;
	if(plposPinpoint2.y > 31)
		plposPinpoint2.y = 31;

	//Lis‰‰ hax
	if(playerPinpointIntersects(plposPinpoint2))
	{
		if(!playerVec[1]->getGodmode())
		{
			if(playerVec[1]->getAlive())
			{
				playerVec[1]->hit(2);

				if(sfx.getStatus() == Sound::Status::Stopped || sfx.getBuffer() == &sm->get(SoundEffects::Pickup))
				{
					sfx.setBuffer(sm->get(SoundEffects::Hit));
					sfx.play();
				}
			}
		}
	}


	//Tarkistusfunktio sille, ett‰ onko pelaaja laavasse
	for(int i = 0; i < WIDTH; i++)
	{
		for(int j = 0; j < HEIGHT; j++)
		{
			if(tilemap2D[j][i].getType() == Lava)
			{				
				if(projectilemanager->getEntityList()->size() != 0)
				{
					for(int k = 0; k < projectilemanager->getEntityList()->size(); k++)
					{
						Vector2f projpos = projectilemanager->getEntityList()->at(k)->getPosition();
						Vector2i pinpointPos = Vector2i((int)floor((projpos.x) / 25), (int)floor(projpos.y / 25));

						if(pinpointPos.x < 0)
							pinpointPos.x = 0;
						if(pinpointPos.y < 0)
							pinpointPos.y = 0;
						if(plposPinpoint.x > 31)
							pinpointPos.x = 31;
						if(plposPinpoint.y > 31)
							pinpointPos.y = 31;
						projectilePinpointIntersects(pinpointPos, projectilemanager->getEntityList()->at(k));
					}
				}
			}

			//Projectilejen intersectit laavan kanssa
			if(powerupmanager->getEntityList()->size() != 0)
			{
				for(int k = 0; k < powerupmanager->getEntityList()->size(); k++)
				{
					if(powerupmanager->getEntityList()->at(k)->getGlobalBounds().intersects(playerVec[0]->getGlobalBounds()))//Intersect = true
					{
						if(powerupmanager->getEntityList()->at(k)->getAlive() && powerupmanager->getEntityList()->at(k)->getHasSpawned())
						{
							powerupmanager->getEntityList()->at(k)->pickup(0);
							sfx.setBuffer(sm->get(SoundEffects::Pickup));
							sfx.play();
						}
					}

					if(powerupmanager->getEntityList()->at(k)->getGlobalBounds().intersects(playerVec[1]->getGlobalBounds()))//Intersect = true
					{
						if(powerupmanager->getEntityList()->at(k)->getAlive() && powerupmanager->getEntityList()->at(k)->getHasSpawned())
						{
							powerupmanager->getEntityList()->at(k)->pickup(1);
							sfx.setBuffer(sm->get(SoundEffects::Pickup));
							sfx.play();
						}
					}

				}
			}

			//tilejen lasku
			switch(tilemap2D[j][i].getType())
			{
				case Lava:
					currentLavaTiles++;
					break;
				case Ground:
					currentGroundTiles++;
					break;
				default:
					cout << "Lava/Ground percentage check failed." << endl;
					break;
			}
			
		}
	}

	//T‰‰ koko hoito on laavan levi‰mist‰ varten
	if(worldUpdateTime >= milliseconds(100.f))
	{
		if(currentGroundTiles < (WIDTH*HEIGHT) / 2)
		{
			floodByGround();
			//cout << "using ground flood" << endl;
		}
		else
		{
			floodByLava();
			//cout << "using lava flood" << endl;
		}
		
		//Poweruppien spawnaus
		if(spawnEnabled)
		{

			r = dev() % pickupChance;
			AmmoPowerup* ammoPowerup = new AmmoPowerup(playerVec, tm, graphicsQuality);

			Vector2u powerupSize = ammoPowerup->getTexture()->getSize();

			//if(r == 0 && playerVec[0]->getAlive() || playerVec[1]->getAlive())
			if (r == 0)
			{
				float a = (dev()%((800-powerupSize.x - 25)-(25 + powerupSize.x)))+(25 + powerupSize.x);
				float b = (dev()%((800-powerupSize.y - 25)-(25 + powerupSize.x)))+(25 + powerupSize.y);
				ammoPowerup->spawn(Vector2f(a, b), 500.f);
				powerupmanager->addEntity(ammoPowerup);
			}
		}

		worldUpdateTime -= milliseconds(100.f);
	}
 
	//Poweruppien erikoiscleanaus
	if(powerupmanager->getEntityList()->size() != 0)
	{
		for(int i = 0; i < powerupmanager->getEntityList()->size(); i++)
		{
			if(powerupmanager->getEntityList()->at(i)->getAlive())
			{
				powerupmanager->getEntityList()->at(i)->update(time);
			}
		}
		powerupmanager->specialCleanEntities();
	}
}

//Algoritmi laavan levi‰miselle. K‰yd‰‰n l‰pi kaikki laavatilet mapissa
void World::floodByLava()
{
	floodDir = rand() % 3;

	switch(floodDir)
	{
		case 0:
			for(int i = WIDTH-1; i >= 0; i--)
			{
				for(int j = HEIGHT-1; j >= 0; j--)
				{
					if(tilemap2D[i][j].getType() == Lava)
					{
						r = dev() % lavaSpreadChance;

						if(r == 0)
						{						
							floodTile(i, j, 0);
						}
					}
				}
			}
			break;

		case 1:
			for(int i = 0; i < WIDTH; i++)
			{
				for(int j = 0; j < HEIGHT; j++)
				{
					if(tilemap2D[i][j].getType() == Lava)
					{
						r = dev() % lavaSpreadChance;

						if(r == 0)
						{						
							floodTile(i, j, 0);
						}
					}
				}
			}
			break;

		case 2:
			for(int i = WIDTH-1; i >= 0; i--)
			{
				for(int j = 0; j < HEIGHT; j++)
				{
					if(tilemap2D[i][j].getType() == Lava)
					{
						r = dev() % lavaSpreadChance;

						if(r == 0)
						{						
							floodTile(i, j, 0);
						}
					}
				}
			}
			break;

		case 3:
			for(int i = 0; i < WIDTH; i++)
			{
				for(int j = HEIGHT-1; j >= 0; j--)
				{
					if(tilemap2D[i][j].getType() == Lava)
					{
						r = dev() % lavaSpreadChance;

						if(r == 0)
						{						
							floodTile(i, j, 0);
						}
					}
				}
			}
			break;
	}
}

//Algoritmi laavan levi‰miselle. K‰yd‰‰n l‰pi kaikki kivitilet mapissa
void World::floodByGround()
{
	floodDir = rand() % 3;

	switch(floodDir)
	{
		case 0:
			for(int i = WIDTH-1; i >= 0; i--)
			{
				for(int j = HEIGHT-1; j >= 0; j--)
				{
					if(tilemap2D[i][j].getType() == Ground)
					{
						r = dev() % lavaSpreadChance;

						if(r == 0)
						{	
							floodTile(i, j, 1);
						}
					}
				}
			}
			break;

		case 1:
			for(int i = 0; i < WIDTH; i++)
			{
				for(int j = 0; j < HEIGHT; j++)
				{
					if(tilemap2D[i][j].getType() == Ground)
					{
						r = dev() % lavaSpreadChance;

						if(r == 0)
						{	
							floodTile(i, j, 1);
						}
					}
				}
			}
			break;

		case 2:
			for(int i = WIDTH-1; i >= 0; i--)
			{
				for(int j = 0; j < HEIGHT; j++)
				{
					if(tilemap2D[i][j].getType() == Ground)
					{
						r = dev() % lavaSpreadChance;

						if(r == 0)
						{	
							floodTile(i, j, 1);
						}
					}
				}
			}
			break;

		case 3:
			for(int i = 0; i < WIDTH; i++)
			{
				for(int j = HEIGHT-1; j >= 0; j--)
				{
					if(tilemap2D[i][j].getType() == Ground)
					{
						r = dev() % lavaSpreadChance;

						if(r == 0)
						{	
							floodTile(i, j, 1);
						}
					}
				}
			}
			break;
	}
}

//Floodaa yhden tilen laavalla jos (i, j) l‰hell‰ laavaa
void World::floodTile(int i, int j, int method) //method 0 = surroundingGrounds, 1 = surroundingLavas
{
	r = rand() % 4;

	if(method == 0)
	{
		switch(r)
		{
			case 0:
				if(i - 1 >= 0)
					if(tilemap2D[i-1][j].getType() == Ground)
					{
						tilemap2D[i-1][j].changeType(Lava);
					}
				break;
			case 1:
				if(i + 1 < WIDTH)
					if(tilemap2D[i+1][j].getType() == Ground)
					{
						tilemap2D[i+1][j].changeType(Lava);
					}
				break;
			case 2:
				if(j - 1 >= 0)
					if(tilemap2D[i][j-1].getType() == Ground)
					{
						tilemap2D[i][j-1].changeType(Lava);
					}
				break;
			case 3:
				if(j + 1 < HEIGHT)
					if(tilemap2D[i][j+1].getType() == Ground)
					{
						tilemap2D[i][j+1].changeType(Lava);
					}
				break;
			default:
				cout << "Random " << r << " failed." << endl;
				break;
		}
	}
	else
	{
		switch(r)
		{
			case 0:
				if(i - 1 >= 0)
					if(tilemap2D[i-1][j].getType() == Lava)
					{
						tilemap2D[i][j].changeType(Lava);
					}
				break;
			case 1:
				if(i + 1 < WIDTH)
					if(tilemap2D[i+1][j].getType() == Lava)
					{
						tilemap2D[i][j].changeType(Lava);
					}
				break;
			case 2:
				if(j - 1 >= 0)
					if(tilemap2D[i][j-1].getType() == Lava)
					{
						tilemap2D[i][j].changeType(Lava);
					}
				break;
			case 3:
				if(j + 1 < HEIGHT)
					if(tilemap2D[i][j+1].getType() ==Lava)
					{
						tilemap2D[i][j].changeType(Lava);
					}
				break;
			default:
				cout << "Random " << r << " failed." << endl;
				break;
		}
	}
}

//Piirret‰‰n kaikki
void World::draw()
{
	for(int i = 0; i < WIDTH; i++)
	{
		for(int j = 0; j < HEIGHT; j++)
		{
			rw->draw(tilemap2D[j][i]);
		}
	}

	if(powerupmanager->getEntityList()->size() != 0)
	{
		for(int i = 0; i < powerupmanager->getEntityList()->size() - 1; i++)
		{
			powerupmanager->getEntityList()->at(i)->drawShadow(rw);
			rw->draw(*powerupmanager->getEntityList()->at(i));			
		}
	}
}

//Charien perusteella tehd‰‰n viel‰ uusi m-dimensional array t‰ynn‰ tilej‰
void World::initalizeWorld()
{
	int l1 = rand() % WIDTH;
	int l2 = rand() % WIDTH;
	int l3 = rand() % WIDTH;
	int l4 = rand() % WIDTH;

	Tile t;
	for(int i = 0; i < WIDTH; i++)
	{
		for(int j = 0; j < HEIGHT; j++)
		{			
			if(charmap2D[j][i] == Lava)
			{
				t = Tile(Lava, tm, graphicsQuality);				
			}
			else if(charmap2D[j][i] == Ground)
			{
				t = Tile(Ground, tm, graphicsQuality);
			}
			else
			{
				cout << j << " | " << j << charmap2D[j][i] << endl;
				t = Tile(Lava, tm, graphicsQuality);
			}
			tilemap2D[j][i] = t;
			tilemap2D[j][i].setPosition((i * TILESIZE), j * TILESIZE);
		}
	}
	
}

//Hax funktio jolla tiedet‰‰n pelaajan t‰sm‰llinen sijainti tilemapissa alkioihin n‰hden
bool World::playerPinpointIntersects(Vector2i pinpointLocation)
{
	if(tilemap2D[pinpointLocation.y][pinpointLocation.x].getGlobalBounds().contains(playerVec[0]->getPosition().x, playerVec[0]->getPosition().y) && tilemap2D[pinpointLocation.y][pinpointLocation.x].getType() == Lava)
	{
		return true;
	}

	if (tilemap2D[pinpointLocation.y][pinpointLocation.x].getGlobalBounds().contains(playerVec[1]->getPosition().x, playerVec[1]->getPosition().y) && tilemap2D[pinpointLocation.y][pinpointLocation.x].getType() == Lava)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Hax funktio jolla tiedet‰‰n projectilen t‰sm‰llinen sijainti tilemapissa alkioihin n‰hden
bool World::projectilePinpointIntersects(Vector2i pinpointLocation, WaterProjectile* projectile)
{
	Vector2i topLeft = Vector2i(pinpointLocation.x - 1, pinpointLocation.y - 1);

	if(pinpointLocation.x == 0 || pinpointLocation.x == 31 || pinpointLocation.y == 0 || pinpointLocation.y == 31)
		return false;

	if(tilemap2D[pinpointLocation.y][pinpointLocation.x].getGlobalBounds().contains(projectile->getPosition().x, projectile->getPosition().y) && tilemap2D[pinpointLocation.y][pinpointLocation.x].getType() == Lava)
	{
		tilemap2D[pinpointLocation.y][pinpointLocation.x].changeType(Ground);
		score->addScore(10);
		return true;
	}
	else
	{
		return false;
	}
}

void World::Reset()
{
	initalizeWorld();
}

int World::getWidth()
{
	return WIDTH;
}

int World::getHeight()
{
	return HEIGHT;
}

int World::getCurrentLavaTiles()
{
	return currentLavaTiles;
}

int World::getCurrentGroundTiles()
{
	return currentGroundTiles;
}

void World::enableSpawn()
{
	this->spawnEnabled = true;
}

bool World::isSpawnEnabled()
{
	return this->spawnEnabled;
}

World::~World()
{
	delete powerupmanager;
	powerupmanager = nullptr;
}

