
#pragma once
#include "ParticleSystem.h"

class RocketFire :public ParticleSystem
{


public:

	GameObject* ParentObject;
	float rot;
	RocketFire();
	void Update(float timeDelta);
	void Draw(BasicSprites::SpriteBatch^ m_spriteBatch);

};