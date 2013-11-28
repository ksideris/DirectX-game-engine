
#pragma once
#include "BaseParticleSystem.h" 

class RocketFire :public ParticleSystem
{


public:

 
	RocketFire();
	void Update(float timeDelta);
	void Draw(BasicSprites::SpriteBatch^ m_spriteBatch);

};