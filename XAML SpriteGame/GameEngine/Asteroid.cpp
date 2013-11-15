#include "pch.h"

#include "Asteroid.h"

void Asteroid::Update(float timeDelta)
{
	GameObject::Update(timeDelta);
//WrapAroundBounds();
}

void Asteroid::WrapAroundBounds()
{

	float2 prev = pos;

	if (pos.x > _windowRect.Width  )
	{
		pos.x = pos.x - _windowRect.Width;
		 
	}
	 
	if (pos.x < 0)
	{
		pos.x = _windowRect.Width+ pos.x  ; 
		 
	}

	UpdateCollisionGeometry(prev, pos, 0);


 
}