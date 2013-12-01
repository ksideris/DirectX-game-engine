
#pragma once 
#include "../../ParticleEffects/FireBall.h"
#include "../Character.h"

enum EnemyMovement{
	RANDOM = 0,
	UP_DOWN = 1
};

class Enemy :public Character
{ 
	float2 TargetPos;
	int hit;
	void Shoot();

public:
	EnemyMovement movementType;

	Enemy(EnemyMovement _moveType);
	void SetTarget(float2 newTarget);
	void Update(float timeDelta);/*
	ImpactResult ProcessHit(float ImpactFactor);*/

	virtual float GetImpactSize();

};