#pragma once

#include "GameObject.h"

class Asteroid :public GameObject
{

public:

	void Update(float timeDelta);
	void WrapAroundBounds();

};