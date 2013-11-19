#include "pch.h"

#include "PassiveObject.h"

void PassiveObject::Update(float timeDelta)
{
	GameObject::Update(timeDelta);
//WrapAroundBounds();
}

void PassiveObject::WrapAroundBounds()
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