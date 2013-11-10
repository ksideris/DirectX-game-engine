#include "pch.h"

#include "Asteroid.h"

void Asteroid::Update(float timeDelta)
{
	GameObject::Update(timeDelta);
	//WrapAroundBounds();
}

void Asteroid::WrapAroundBounds()
{

	if (pos.x > _windowRect.Width + textureSize.Width  * scale.x)
	{
		pos.x = 0;
		accel.x = 0;
		vel.x = 0;
	}
	if (pos.y > _windowRect.Height + textureSize.Height   * scale.y)
	{
		pos.y = 0;
		accel.y = 0;
		vel.y = 0;
	}
	if (pos.x < textureSize.Width  * scale.x)
	{
		pos.x = _windowRect.Width + textureSize.Width   * scale.x;
		accel.x = 0;
		vel.x = 0;
	}
	if (pos.y < textureSize.Height   * scale.y)
	{
		pos.y = _windowRect.Height + textureSize.Height   * scale.y;
		accel.y = 0;
		vel.y = 0;
	}


 
}