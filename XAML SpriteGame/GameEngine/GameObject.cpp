

#include "pch.h"
#include "GameObject.h"

using namespace BasicSprites;

float2 GameObject::GetTopLeft(){
	return float2(pos.x - textureSize.Width / 2.0f*scale.x, pos.y - textureSize.Height / 2.0f*scale.y);
}
float2 GameObject::GetBottomRight(){
	return float2(pos.x + textureSize.Width / 2.0f*scale.x, pos.y + textureSize.Height / 2.0f*scale.y);
}
 

void	GameObject::SetVel(float2 _vel){
	vel = _vel;
}
float2  GameObject::GetVel(){
	return vel;
}
void    GameObject::SetAccel(float2 _accel){
	accel = _accel;
}
float2  GameObject::GetAccel(){
	return accel;
}
void    GameObject::SetRotVel(float  _rotVel){
	rotVel = _rotVel;
}
float   GameObject::GetRotVel(){
	return rotVel;
}



void GameObject::Update(float timeDelta)
{


	float  prevRot = rot;
	vel = vel + accel*timeDelta;
	float2   prevPos = pos;
	pos = pos + vel * timeDelta;

	rot = rot + rotVel * timeDelta;
	if (rot > PI_F)
	{
		rot -= 2.0f * PI_F;
	}
	if (rot < -PI_F)
	{
		rot += 2.0f * PI_F;
	}

	UpdateChildren(timeDelta);
	UpdateCollisionGeometry(prevPos, pos, prevRot-rot);

}
void GameObject::UpdateChildren(float timeDelta)
{

	for (auto child = children.begin(); child != children.end(); child++)
	{
		float2 offset = child->second;
		child->first->SetPos(float2(pos.x - offset.x *cos(rot), pos.y + offset.y*sin(rot)));
		child->first->SetRot(rot);
		child->first->Update(timeDelta);
	}
}
void GameObject::UpdateCollisionGeometry(float2 prevPos, float2 pos, float rot)
{
	translateCollisionGeometry(pos - prevPos);
	rotateCollisionGeometry(rot, pos);
 
}
void GameObject::AddChild(float2 offset, Sprite* obj)
{
	children.push_back(std::pair< Sprite*, float2 >(obj, offset));
}


void GameObject::Draw(BasicSprites::SpriteBatch^ m_spriteBatch)
{
	for (auto child = children.begin(); child != children.end(); child++)
	{
		child->first->Draw(m_spriteBatch);
	}

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

void GameObject::SetTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D>  texture)
{
	Sprite::SetTexture(texture);

	setRectangleCollisionGeometry(GetTopLeft(), GetBottomRight());

}
