

#include "pch.h"
#include "GameObject.h"

using namespace BasicSprites;

float2 GameObject::GetTopLeft(){ 
	return float2(pos.x - textureSize.Width / 2.0*scale, pos.y - textureSize.Height / 2.0*scale);
}
float2 GameObject::GetBottomRight(){
	return float2(pos.x + textureSize.Width / 2.0*scale, pos.y + textureSize.Height / 2.0*scale);
}

bool GameObject::IsColliding(GameObject  otherObj)
{

	float2 tl1 = GetTopLeft();
	float2 tl2 = otherObj.GetTopLeft();
	float2 br1 = GetBottomRight();
	float2 br2 = otherObj.GetBottomRight();

	if (br1.y  < tl2.y) return(0);
	if (tl1.y  > br2.y) return(0);

	if (br1.x < tl2.x) return(0);
	if (tl1.x > br2.x) return(0);

	return 1;

}
 
void GameObject::Update(float timeDelta)
{


	//vel = vel + accel*timeDelta;
	pos = pos + vel * timeDelta;
 
	rot = rot+ rotVel * timeDelta;
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
 