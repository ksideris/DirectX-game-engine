

#include "pch.h"
#include "Player.h"

using namespace Windows::System;
using namespace Windows::UI::Core;


Player::Player()
{
	keys_down = 0;
	lightAngle = 0;
	lightUpdate = 50;
}
void Player::Update(float timeDelta)
{ 

	pos = pos + float2(vel.x*cos(rot), -vel.x*sin(rot)) * timeDelta;

	rot = rot + rotVel * timeDelta;
	if (rot > PI_F)
	{
		rot -= 2.0f * PI_F;
	}
	if (rot < -PI_F)
	{
		rot += 2.0f * PI_F;
	}


	if (lightAngle > 45)
		lightUpdate = -50;
	else if (lightAngle < -45)
		lightUpdate = 50;

	lightAngle = lightAngle + lightUpdate * timeDelta;

	KeepInBounds();
}
void Player::KeepInBounds()
{

	if (pos.x > _windowRect.Width - textureSize.Width / 2.0 * scale)
	{
		pos.x = _windowRect.Width - textureSize.Width / 2.0 * scale;
		accel.x = 0;
		vel.x = 0;
	}
	if (pos.y > _windowRect.Height - textureSize.Height / 2.0 * scale)
	{
		pos.y = _windowRect.Height - textureSize.Height / 2.0 * scale;
		accel.y = 0;
		vel.y = 0;
	}
	if (pos.x < textureSize.Width / 2.0 * scale)
	{
		pos.x = textureSize.Width / 2.0 * scale;
		accel.x = 0;
		vel.x = 0;
	}
	if (pos.y < textureSize.Height / 2.0 * scale)
	{
		pos.y = textureSize.Height / 2.0 * scale;
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
	if ( ! args->KeyStatus.WasKeyDown )
		keys_down += 1;
}
void Player::ProcessKeyUp(Windows::UI::Xaml::Input::KeyRoutedEventArgs^ args){
	if (args->Key == VirtualKey::Left || args->Key == VirtualKey::Right)
			rotVel = 0;
	
	keys_down -= 1;
	if (keys_down == 0)
		vel = (0, 0);
}

void Player::Draw(BasicSprites::SpriteBatch^ m_spriteBatch)
{
	float2 spot_pos = float2(pos.x + (-10 + textureSize.Width / 2.0* scale)*cos(rot), pos.y - (-10 + textureSize.Width / 2.0* scale)*sin(rot));
	//+(lightAngle)*3.14 / 180.0
	m_spriteBatch->Draw(
		spot_texture.Get(),
		spot_pos,
		PositionUnits::DIPs,
		float2(1.1f, 1.1f) * scale,
		SizeUnits::Normalized,
		float4(0.98f, 0.8f, 1.0f, 1.0f),
		rot
		);

	GameObject::Draw(m_spriteBatch);


}
