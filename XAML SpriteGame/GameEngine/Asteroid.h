#pragma once

#include "GameObject.h"

class Asteroid :public GameObject
{

public:
	float lifeTime;
	void Update(float timeDelta);
	void WrapAroundBounds();

};