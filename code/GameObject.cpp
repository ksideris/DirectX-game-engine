

#include "pch.h"
#include "GameObject.h"

using namespace BasicSprites;


bool GameObject::IsColliding(float2  otherPos)
{
 
	if (sqrt(pow(otherPos.x - pos.x, 2) + pow(otherPos.y - pos.y, 2)) < size.x )
		return 1;

	else 
		return 0;
	  
}
 
void GameObject::Update(float timeDelta)
{


	//vel = vel + accel*timeDelta;
	pos = pos + vel * timeDelta;


	rot += rotVel * timeDelta;
	if (rot > PI_F)
	{
		rot -= 2.0f * PI_F;
	}
	if (rot < -PI_F)
	{
		rot += 2.0f * PI_F;
	}


}

void GameObject::Draw(BasicSprites::SpriteBatch^ m_spriteBatch)
{
	m_spriteBatch->Draw(
		_texture.Get(),
		pos,
		PositionUnits::DIPs,
		float2(1.0f, 1.0f) * scale,
		SizeUnits::Normalized,
		float4(0.8f, 0.8f, 1.0f, 1.0f),
		rot
		);
}