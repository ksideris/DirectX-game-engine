#pragma once
#include "Sprite.h"

using namespace Coding4Fun::FallFury::DXCore;
using namespace Coding4Fun::FallFury::DXCore::BasicSprites;
class ParticleSystem :public Sprite
{
protected:

	int numOfParticles;
	std::vector<float4> particles;
public: 
	
	float2 vel;  
	ParticleSystem();
	void Update(float timeDelta);
	void Draw(BasicSprites::SpriteBatch^ m_spriteBatch);


	float RandFloat(float min, float max);

};