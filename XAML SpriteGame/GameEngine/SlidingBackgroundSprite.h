
#pragma once
#include "Sprite.h"


class SlidingBackgroundSprite : public Sprite
{
	float vel;
	float2 second_pos;

public:
	void InitSliding();
	void Update(float timeDelta);
	void Draw(BasicSprites::SpriteBatch^ m_spriteBatch);
};