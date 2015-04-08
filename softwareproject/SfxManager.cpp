#include "SfxManager.h"


void SfxManager::load(SoundEffects::ID id, const string& filename) {
	unique_ptr<SoundBuffer> sound(new SoundBuffer()); //luodaan uusi sound-niminen uniikki pointeri tekstuuriin
	if (!sound->loadFromFile(filename)) //loadFromFile etsii filenamella olevaa ääntä, palauttaa falsen mikäli tekstuurin lataus ei onnistu, silloin tulee heittää poikkeus
		throw
	runtime_error("Hitsi, SfxHolder::load ei onnistunut lataamaan tiedostoa " + filename);
	auto inserted = mSoundMap.insert(make_pair(id, move(sound))); //annetaan teksuurille oma id, ja laitetaan se teksuurimappiin. inserted-muuttuja on olemassa varmuuden varalta, jottei samaa id:tä käytetä kahdesti

	//assert(inserted.second); //mapin insert-metodi palauttaa insertatun asian alkion numeron sekä bool-muuttujan joka on true jos inserttaus onnistui
}

SoundBuffer& SfxManager::get(SoundEffects::ID id)
{
	auto found = mSoundMap.find(id);//etsitään id:n perusteella teksuurimapista tarvittava tekstuuri. auto-tyypitys on c++ 11 uusi ominaisuus, compiler osaa itse päätellä tarvittavan muuttujatyypin
	assert(found != mSoundMap.end()); //tarkastetaan josko kyseisellä id:llä löytyy mapista mitään.
	return *found->second; //palautetaan löydetyn pointerin toinen jäsen (pelkkä tekstuuri itse)
}