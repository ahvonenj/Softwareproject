#include "TextureManager.h"

void TextureManager::load(Textures::ID id, const string& filename) {
	unique_ptr<Texture> texture(new Texture()); //luodaan uusi texture-niminen uniikki pointeri tekstuuriin
	if (!texture->loadFromFile(filename)) //loadFromFile etsii filenamella olevaa kuvaa, palauttaa falsen mik�li tekstuurin lataus ei onnistu, silloin tulee heitt�� poikkeus
		throw
	runtime_error("Hitsi, TextureHolder::load ei onnistunut lataamaan tiedostoa " + filename);
	auto inserted = mTextureMap.insert(make_pair(id, move(texture))); //annetaan teksuurille oma id, ja laitetaan se teksuurimappiin. inserted-muuttuja on olemassa varmuuden varalta, jottei samaa id:t� k�ytet� kahdesti

	//assert(inserted.second); //mapin insert-metodi palauttaa insertatun asian alkion numeron sek� bool-muuttujan joka on true jos inserttaus onnistui
}

Texture& TextureManager::get(Textures::ID id)
{
	auto found = mTextureMap.find(id);//etsit��n id:n perusteella teksuurimapista tarvittava tekstuuri. auto-tyypitys on c++ 11 uusi ominaisuus, compiler osaa itse p��tell� tarvittavan muuttujatyypin
	assert(found != mTextureMap.end()); //tarkastetaan josko kyseisell� id:ll� l�ytyy mapista mit��n.
	return *found->second; //palautetaan l�ydetyn pointerin toinen j�sen (pelkk� tekstuuri itse)
}