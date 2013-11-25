#pragma once
#include "Sprite.h"  
#include "ParticleSystem.h"  
#include "CollidingObject.h"
 

class FireBall :public ParticleSystem, public CollidingObject
{
protected:
	 
public: 
	float4 color;
	FireBall(bool _isFlipped);
	virtual  void Update(float timeDelta);
	virtual void Draw(BasicSprites::SpriteBatch^ m_spriteBatch);
	bool isFlipped;
 
	float2 GetTopLeft();
	float2 GetBottomRight();
	void UpdateCollisionGeometry(float2 prevPos, float2 pos, float rot);
	void setCollisionGeometryForParticle(float2 size, float2 _pos);

};