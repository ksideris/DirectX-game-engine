#pragma once
#include "../Sprite.h" 
#include "../GameObject.h" 

using namespace  DXCore;
using namespace DXCore::BasicSprites;


using namespace AudioEngine;



class ParticleSystem :public GameObject
{
protected:

	int numOfParticles;
	std::vector<float4> particles;

public: 
	float2 vel;

	ParticleSystem(){}

	virtual  void Update(float timeDelta) = 0;
	virtual  void Draw(BasicSprites::SpriteBatch^ m_spriteBatch) = 0;

 
}; 