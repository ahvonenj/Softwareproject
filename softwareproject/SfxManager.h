
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <utility>
#include <assert.h>


using namespace std;
using namespace sf;

//nimiavaruus tekstuureille, jokaiselle pelin objektille oma ID
namespace SoundEffects 
{
	enum ID 
	{
		Shoot, Hit, Pickup
	};
}

class SfxManager {
	
	private:
		map<SoundEffects::ID, //tehd��n tekstuureille oma mappi nimelt� mTextureMap, k�ytt�en C++ 11:n fiksuja pointereita
			unique_ptr<SoundBuffer>> mSoundMap;


	public:

		void load(SoundEffects::ID id, const string& filename);
		SoundBuffer& get(SoundEffects::ID id);
		~SfxManager(){}
};
