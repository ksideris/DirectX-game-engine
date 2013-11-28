#pragma once
 
#include "BaseParticleSystem.h" 

class Explosion :public ParticleSystem
{
public:

	float4 color;
	int lifetime;
	Explosion();
	Explosion(float2 _size, float4 _color);
	void Update(float timeDelta);
	void Draw(BasicSprites::SpriteBatch^ m_spriteBatch);
};