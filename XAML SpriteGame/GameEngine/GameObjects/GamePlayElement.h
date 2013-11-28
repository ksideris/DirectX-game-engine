#pragma once

#include "../GameObject.h"

enum GamePlayType
{
	RING = 1,
	ASTEROID = 2, 
	UPGRADE = 4,
	HEALTH = 5,
	FINISHLINE = 6

};

class GamePlayElement :public GameObject
{
	float _lifeTime;

public:
	GamePlayType type;

	void Update(float timeDelta);
	void WrapAroundBounds();
	ImpactResult ProcessHit(float ImpactFactor);

	float GetImpactSize();
};