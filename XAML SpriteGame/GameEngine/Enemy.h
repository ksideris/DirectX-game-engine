
#pragma once 
#include "FireBall.h"
#include "GameObject.h"

enum EnemyMovement{
	RANDOM = 0,
	UP_DOWN = 1
};

class Enemy :public GameObject
{
	float lastFired;
	float2 TargetPos;
	float timeAlive;
	int hit = 0;
	void Shoot();
public:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>		 _projectile;
	std::vector<FireBall> bullets;
	EnemyMovement movementType;
	int health;
	Enemy(EnemyMovement _moveType);
	void SetTarget(float2 newTarget);
	void Update(float timeDelta);
	void KeepInBounds();
	void ProcessHit(float ImpactFactor);
	void Draw(BasicSprites::SpriteBatch^ m_spriteBatch);
};