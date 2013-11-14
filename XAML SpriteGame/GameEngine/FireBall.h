#pragma once
#include "Sprite.h"  
#include "ParticleSystem.h"  
#include "CollidingObject.h"
 

class FireBall :public ParticleSystem, public CollidingObject
{
protected:
	 
public: 
	FireBall();
	virtual  void Update(float timeDelta);
	virtual void Draw(BasicSprites::SpriteBatch^ m_spriteBatch);
	 
 
	float2 GetTopLeft();
	float2 GetBottomRight();
	void UpdateCollisionGeometry(float2 prevPos, float2 pos, float rot);
	void setCollisionGeometryForParticle(float2 size, float2 _pos);

};