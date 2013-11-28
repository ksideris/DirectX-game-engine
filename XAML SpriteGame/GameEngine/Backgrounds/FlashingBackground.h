
#pragma once
#include "../Background.h"



class FlashingBackground : public Background
{
	float4 color;
public:
	FlashingBackground();
	void Update(float timeDelta);
	void Draw(BasicSprites::SpriteBatch^ m_spriteBatch); 

};