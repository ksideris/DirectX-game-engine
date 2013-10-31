#pragma once
#include "Sprite.h" 
#include "GameObject.h" 

using namespace Coding4Fun::FallFury::DXCore;
using namespace Coding4Fun::FallFury::DXCore::BasicSprites;


class ParticleSystem :public Sprite
{
protected:

	int numOfParticles;
	std::vector<float4> particles;
public: 
	float2 size;
	float2 vel;  
	ParticleSystem();
	void Update(float timeDelta);
	void Draw(BasicSprites::SpriteBatch^ m_spriteBatch);


	float RandFloat(float min, float max);
	bool IsColliding(GameObject  otherObj);  
	float2 GetTopLeft();
	float2 GetBottomRight();

};