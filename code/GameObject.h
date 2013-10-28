#pragma once

#include "HelperClasses.h"
#include "DirectXBase.h"
#include "BasicSprites.h"
#include "Sprite.h"

class GameObject: public Sprite
{

public: 
	float2 vel;
	float rot;
	float rotVel;
	float scale;
	float2 accel;

	float2 size; 
	 

	void Draw(BasicSprites::SpriteBatch^ m_spriteBatch);
	bool IsColliding(float2  otherPos );
	void Update(float timeDelta);
	void KeepInBounds();
};
 