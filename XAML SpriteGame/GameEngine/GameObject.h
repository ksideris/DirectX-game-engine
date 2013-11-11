#pragma once

#include <utility> 
#include "Sprite.h"
#include "CollidingObject.h"

using namespace std;
using namespace  DXCore;
using namespace  DXCore::BasicSprites;

class GameObject : public Sprite, public CollidingObject
{
protected:

	float  rotVel;
	float2 accel;
	float2 vel;
	vector<pair<Sprite*, float2> > children;
	virtual void UpdateChildren(float timeDelta);
	void UpdateCollisionGeometry(float2 prevPos, float2 pos, float rot); 

public:
	 

	float2 size;

	virtual void    SetVel(float2 _vel);  //Convert To properties!
	virtual float2  GetVel();
	virtual void    SetAccel(float2 _accel);
	virtual float2  GetAccel();
	virtual void    SetRotVel(float  _rotVel);
	virtual float   GetRotVel();

	virtual void Update(float timeDelta);
	virtual void Draw(BasicSprites::SpriteBatch^ m_spriteBatch);

	

	virtual void AddChild(float2 offset, Sprite* obj);
	void KeepInBounds(); 

	virtual void SetTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D>  texture);
	float2 GetTopLeft();
	float2 GetBottomRight();
};
