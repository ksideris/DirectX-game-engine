

#include "pch.h"
#include "Player.h"

using namespace Windows::System;
using namespace Windows::UI::Core;


Player::Player()
{
	health = 100;
	TargetPos = (-1.f, -1.f);
	keys_down = 0;  
	z = 25;
}
void Player::Update(float timeDelta)
{

	float2 prevPos = pos;
	float  prevRot = rot;
	if (TargetPos.x != -1)
	{
		float angle;

		if (TargetPos.y - pos.y < 0)
			angle = atan2(abs(TargetPos.y - pos.y), (TargetPos.x - pos.x)) ;
		else
			angle = atan2(-abs(TargetPos.y - pos.y), (TargetPos.x - pos.x))  ;
 

		angle -= rot;
		if ((180 * (angle) / PI_F)   >300)
			angle -= 2 * 3.14f;
		if ((180 * (angle) / PI_F) < -300)
			angle += 2 * 3.14f;


		if (abs(angle) < .05)
			pos = pos + float2(TargetPos.x - pos.x, TargetPos.y - pos.y) * timeDelta;
		 rotVel = (PI_F*angle/180  )* 1000;
		 
		
	 }
	else
		pos = pos + float2(vel.x*cos(rot), -vel.x*sin(rot)) * timeDelta;

	prevRot = rot;
	rot = rot + rotVel * timeDelta;
	if (rot > PI_F)
	{
		rot -= 2.0f * PI_F;
	}
	if (rot < -PI_F)
	{
		rot += 2.0f * PI_F;
	}
	 
	KeepInBounds(); 


	UpdateChildren(timeDelta);
	UpdateCollisionGeometry(prevPos, pos, prevRot- rot  );
	 


}
void Player::KeepInBounds()
{

	if (pos.x > _windowRect.Width - textureSize.Width / 2.0f * scale.x)
	{
		pos.x = _windowRect.Width - textureSize.Width / 2.0f * scale.x;
		accel.x = 0;
		vel.x = 0;
	}
	if (pos.y > _windowRect.Height - textureSize.Height / 2.0f * scale.y)
	{
		pos.y = _windowRect.Height - textureSize.Height / 2.0f * scale.y;
		accel.y = 0;
		vel.y = 0;
	}
	if (pos.x < textureSize.Width / 2.0f * scale.x)
	{
		pos.x = textureSize.Width / 2.0f * scale.x;
		accel.x = 0;
		vel.x = 0;
	}
	if (pos.y < textureSize.Height / 2.0f * scale.y)
	{
		pos.y = textureSize.Height / 2.0f * scale.y;
		accel.y = 0;
		vel.y = 0;
	} 
}


void Player::ProcessKeyDown(Windows::UI::Xaml::Input::KeyRoutedEventArgs^ args){

	if (args->Key == VirtualKey::Right)
		rotVel = -2;
	if (args->Key == VirtualKey::Left)
		rotVel =  2;
		
	if (args->Key == VirtualKey::Up)
	{

		vel.x = 500;
	}
	if (args->Key == VirtualKey::Down)
	{
		vel.x = -500;
	}

	if (args->Key == VirtualKey::W)
	{
		z += 5;
		scale = scale + float2(.001f, .001f);
	}
	if (args->Key == VirtualKey::Q)
	{
		z -= 5;
		scale = scale- float2(.001f, .001f);
	}
	if ( ! args->KeyStatus.WasKeyDown )
		keys_down += 1;

	TargetPos = float2(-1, -1);
}
void Player::ProcessKeyUp(Windows::UI::Xaml::Input::KeyRoutedEventArgs^ args){
	if (args->Key == VirtualKey::Left || args->Key == VirtualKey::Right)
			rotVel = 0;
	
	keys_down -= 1;
	if (keys_down == 0)
		vel = (0.f, 0.f);
}

void Player::Draw(BasicSprites::SpriteBatch^ m_spriteBatch)
{
 

	GameObject::Draw(m_spriteBatch);


}



void Player::ProcessPointerPressed(Windows::UI::Input::PointerPoint^ pt)
{ 

	TargetPos = float2(pt->Position.X, pt->Position.Y);

		 
 
}
void Player::ProcessPointerReleased(Windows::UI::Input::PointerPoint^ pt)
{


	TargetPos = float2(-1, -1);
	vel.x = 0;
	rotVel = 0;
	 
}
void Player::ProcessPointerMoved(Windows::UI::Input::PointerPoint^ pt)
{ 
	TargetPos = float2(pt->Position.X, pt->Position.Y);
  
}
 