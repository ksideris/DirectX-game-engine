#pragma once

#include <utility> 
#include "Sprite.h"
#include "CollidingObject.h"
 

using namespace std;
using namespace  DXCore;
using namespace  DXCore::BasicSprites;

enum ImpactResult
{
	score = 1,
	finish =2,
	hit = 3,
	disappear = 4,
	death = 5,
	explosion = 6,
	bigexplosion = 7,
	shatter = 8,
	health = 9,
	weapon_upgrade = 10,

};

class GameObject : public Sprite, public CollidingObject
{
protected:

	float  rotVel;
	float2 accel;
	float2 vel;

	vector<pair<Sprite*, float2> > children;

	virtual void UpdateChildren(float timeDelta);
	void UpdateCollisionGeometry(float2 prevPos, float2 pos, float rot); 
	virtual void UpdateLifeTime();

	float _lifetime;
	bool _dead;

public:

	GameObject();

	virtual bool	IsAlive();
	virtual void	SetDead();

	virtual float	GetLifeTime();
	virtual void	SetLifeTime(float newLifeTime);

	virtual void    SetVel(float2 _vel);  
	virtual float2  GetVel();
	virtual void    SetAccel(float2 _accel);
	virtual float2  GetAccel();
	virtual void    SetRotVel(float  _rotVel);
	virtual float   GetRotVel();
	virtual float GetImpactSize();


	virtual void Update(float timeDelta);
	virtual void Draw(BasicSprites::SpriteBatch^ m_spriteBatch);
	virtual void AddChild(float2 offset, Sprite* obj);
	 

	virtual void SetTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D>  texture);

	virtual ImpactResult ProcessHit(float ImpactFactor);

	float2 GetTopLeft();
	float2 GetBottomRight();
};
