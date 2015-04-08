#include "Scene.h"
#include "Game.h"
#include "Menu.h"
#include <vector>
#include <string>

int main(int argc, char *argv[])
{
	string versionHash = "9d9eb437a699cf73b37f604173a1264f"; //tarkastetaan serveripuolella onko pelaajalla uusin versio pelistä

	RenderWindow *renderwindow;
	if(*argv[argc-1] == 'L')
	{
		renderwindow = new RenderWindow(VideoMode(1280, 800), "Softwareproject", Style::Default, ContextSettings(0U, 0U, 2U, 2U, 0U));
	}
	else if(*argv[argc-1] == 'H')
	{
		renderwindow = new RenderWindow(VideoMode(1280, 800), "Softwareproject", Style::Default);
	}
	else
	{
		renderwindow = new RenderWindow(VideoMode(1280, 800), "Softwareproject", Style::Default);
	}

	//RenderWindow *renderwindow = new RenderWindow(VideoMode(1280, 800), "Softwareproject", Style::Close);
	std::vector<Scene*> Scenes;
	int scene = 0;

	char argv_a; //argumentit että onko multiplayer/high graphics
	char argv_b;

	if(argc == 0)
	{
		argv_a = 'H';
		argv_b = '1';
	}
	else if(argc == 1)
	{
		argv_a = *argv[argc-1];
		argv_b = '1';
	}
	else
	{
		argv_a = *argv[argc-2];
		argv_b = *argv[argc-1];
	}

	Game game(argv_a, renderwindow, versionHash, argv_b); //luodaan uusi game ja menu
	Menu menu(renderwindow, versionHash);
	
	Scenes.push_back(&menu); //ja laitetaan ne scene-vektoriin
	Scenes.push_back(&game);

	game.setGametype(1);

	//Päälooppi, pyöritellään gamen ja menun Runia aina vuorollaan. 0-alkio on menu, 1-alkio on game. jos run palauttaa 2 on kyseessä custom-game
	while (scene >= 0)
	{
		scene = Scenes[scene]->Run();

		if(scene == 2)
		{
			scene = 1;
			game.setGametype(0);
		}
		else
		{
			game.setGametype(1);
		}
	}

	//jonkin runin palautettua -1 päätetään ohjelma
	renderwindow->close();
	delete renderwindow;
	renderwindow = nullptr;
	return EXIT_SUCCESS;

}


// http://tinyurl.com/aksjhiglhotphibflhktp

