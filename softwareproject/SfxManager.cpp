#include "SfxManager.h"


void SfxManager::load(SoundEffects::ID id, const string& filename) {
	unique_ptr<SoundBuffer> sound(new SoundBuffer()); //luodaan uusi sound-niminen uniikki pointeri tekstuuriin
	if (!sound->loadFromFile(filename)) //loadFromFile etsii filenamella olevaa ��nt�, palauttaa falsen mik�li tekstuurin lataus ei onnistu, silloin tulee heitt�� poikkeus
		throw
	runtime_error("Hitsi, SfxHolder::load ei onnistunut lataamaan tiedostoa " + filename);
	auto inserted = mSoundMap.insert(make_pair(id, move(sound))); //annetaan teksuurille oma id, ja laitetaan se teksuurimappiin. inserted-muuttuja on olemassa varmuuden varalta, jottei samaa id:t� k�ytet� kahdesti

	//assert(inserted.second); //mapin insert-metodi palauttaa insertatun asian alkion numeron sek� bool-muuttujan joka on true jos inserttaus onnistui
}

SoundBuffer& SfxManager::get(SoundEffects::ID id)
{
	auto found = mSoundMap.find(id);//etsit��n id:n perusteella teksuurimapista tarvittava tekstuuri. auto-tyypitys on c++ 11 uusi ominaisuus, compiler osaa itse p��tell� tarvittavan muuttujatyypin
	assert(found != mSoundMap.end()); //tarkastetaan josko kyseisell� id:ll� l�ytyy mapista mit��n.
	return *found->second; //palautetaan l�ydetyn pointerin toinen j�sen (pelkk� tekstuuri itse)
}