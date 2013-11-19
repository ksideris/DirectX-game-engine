#pragma once

#include "GameObject.h"

enum PassiveObjectType
{
	RING = 1,
	ASTEROID = 2,
	FRAGMENT = 3,
	UPGRADE = 4

};

class PassiveObject :public GameObject
{

public:
	PassiveObjectType type;
	float lifeTime;

	void Update(float timeDelta);
	void WrapAroundBounds();

};