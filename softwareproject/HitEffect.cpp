#include "HitEffect.h"


HitEffect::HitEffect() : drawEffect(false)
{
	this->setSize(Vector2f(2000.f, 2000.f));
	this->setPosition(-500, -500);
	this->setFillColor(Color(255, 0, 0, 40));
}

Time displayTime;

void HitEffect::hide(Time time)
{
	displayTime += time;

	if(displayTime >= milliseconds(100.f))
	{
		drawEffect = false;
		displayTime -= milliseconds(100.f);
	}
}

void HitEffect::setDrawEffect(bool drawEffect)
{
	this->drawEffect = drawEffect;
}

bool HitEffect::getDrawEffect()
{
	return this->drawEffect;
}

HitEffect::~HitEffect()
{
}
