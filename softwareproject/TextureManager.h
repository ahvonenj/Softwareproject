

#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <utility>
#include <assert.h>

using namespace std;
using namespace sf;

//nimiavaruus tekstuureille, jokaiselle pelin objektille oma ID
namespace Textures 
{
	enum ID
	{
		LavaTexture,
		GroundTexture,
		WaterTexture,
		MieskoHealthy,
		MieskoBruised,
		MieskoDead,
		P2MieskoHealthy,
		P2MieskoBruised,
		P2MieskoDead,
		background_a,
		Blank,
		Pixel,
		Droplet,
		Player,
		Player2
	};
}

class TextureManager {
	
	private:
		map<Textures::ID, //tehdään tekstuureille oma mappi nimeltä mTextureMap, käyttäen C++ 11:n fiksuja pointereita
			unique_ptr<Texture>> mTextureMap;
	public:

		void load(Textures::ID id, const string& filename);
		Texture& get(Textures::ID id);
		~TextureManager(){}

};
