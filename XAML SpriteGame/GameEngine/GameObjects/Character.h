
#pragma once
#include "../GameObject.h"
#include "../ParticleEffects/FireBall.h"
#include "../GlobalData.h"


using namespace DXCore;
using namespace DXCore::BasicSprites;


class Character :public GameObject
{ 
	float2 TargetPos;
	int hit;
protected:
	virtual void Shoot();
public:

	int health;

	Character(); 

	Microsoft::WRL::ComPtr<ID3D11Texture2D> spot_texture;
 
	virtual void Reset();
	virtual void Update(float timeDelta)=0;
	virtual void SetTarget(float2 newTarget);
	void KeepInBounds(); 
	virtual  void Draw(BasicSprites::SpriteBatch^ m_spriteBatch);
	void UpdateChildren(float timeDelta);
	ImpactResult ProcessHit(float ImpactFactor);

}; 
 
