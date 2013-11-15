
#pragma once 
#include "GameObject.h"

class Enemy :public GameObject
{
	float lastFired ;
public:
	Enemy();
	void Update(float timeDelta);
};