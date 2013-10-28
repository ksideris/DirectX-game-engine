#pragma once

#include "HelperClasses.h" 
#include "Sprite.h"

using namespace Coding4Fun::FallFury::DXCore;
using namespace Coding4Fun::FallFury::DXCore::BasicSprites;

class GameObject: public Sprite
{

public: 
	float2 vel;
	float rot;
	float rotVel; 
	float2 accel;

	float2 size; 
	 

	void Draw(BasicSprites::SpriteBatch^ m_spriteBatch);
	bool IsColliding(GameObject  otherObj);
	void Update(float timeDelta);
	void KeepInBounds();
	float2 GetTopLeft();
	float2 GetBottomRight();
};
 