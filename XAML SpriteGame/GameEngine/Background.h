
#pragma once
#include "Sprite.h" 

enum BackgroundRenderMode{
	ORIGINAL = 1,
	STRETCH_HEIGHT = 2,
	STRETCH_WIDTH = 3,
	STRETCH = 4
};

class  Background : public Sprite
{
protected:


	BackgroundRenderMode rendermode;
public:
	
	virtual void Update(float timeDelta)=0;
	virtual void Draw(BasicSprites::SpriteBatch^ m_spriteBatch) = 0; 

};