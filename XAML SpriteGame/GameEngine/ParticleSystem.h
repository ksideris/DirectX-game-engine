#pragma once
#include "Sprite.h" 
#include "GameObject.h" 

using namespace  DXCore;
using namespace DXCore::BasicSprites;


using namespace AudioEngine;



class ParticleSystem :public Sprite
{
protected:

	int numOfParticles;
	std::vector<float4> particles;
public:
	float2 size;
	float2 vel;

	ParticleSystem(){}

	virtual  void Update(float timeDelta) = 0;
	virtual void Draw(BasicSprites::SpriteBatch^ m_spriteBatch) = 0;


	float RandFloat(float min, float max)
	{
		return (static_cast<float>(rand() % RAND_MAX) / static_cast<float>(RAND_MAX)) * (max - min) + min;
	}

}; 