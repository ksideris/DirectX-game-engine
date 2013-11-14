
#pragma once
#include "Sprite.h"


class SlidingBackgroundSprite : public Sprite
{
	float2 vel;
	float2 second_pos;

public:
	void InitSliding();
	void Update(float timeDelta);
	void Draw(BasicSprites::SpriteBatch^ m_spriteBatch);
	virtual void    SetVel(float2 _vel);  //Convert To properties!
	virtual float2  GetVel();
};