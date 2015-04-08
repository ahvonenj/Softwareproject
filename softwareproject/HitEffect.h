#pragma once
#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>

using namespace sf;

class HitEffect : public RectangleShape
{
private:
	bool drawEffect;

public:
	HitEffect();
	~HitEffect();
	void hide(Time time);
	bool getDrawEffect();
	void setDrawEffect(bool drawEffect);
};

