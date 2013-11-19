#pragma once

#include "Sprite.h"


using namespace DXCore;
using namespace  DXCore::BasicSprites;


class Ground : public Sprite
{
public:
	std::vector< std::pair<float,float> > heightMap;


	virtual void Draw(BasicSprites::SpriteBatch^ m_spriteBatch,float CurrLoc);

};