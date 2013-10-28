#include "pch.h"
#include "BackgroundSprite.h"


void BackgroundSprite::Update(float timeDelta)
{
	pos.x -= vel;
}