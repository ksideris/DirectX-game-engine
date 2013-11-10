
#pragma once
#include "ParticleSystem.h"
#include "CollidingObject.h"

class RocketFire :public ParticleSystem
{


public:

	GameObject* ParentObject;
 
	RocketFire();
	void Update(float timeDelta);
	void Draw(BasicSprites::SpriteBatch^ m_spriteBatch);

};