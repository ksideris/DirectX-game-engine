

#include "pch.h"
#include "Player.h"

using namespace Windows::System;
using namespace Windows::UI::Core;

void Player::Update(float timeDelta)
{
	GameObject::Update(timeDelta);
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


void Player::ProcessKeyDown(Windows::UI::Core::KeyEventArgs^ args){

	if (args->VirtualKey == VirtualKey::Right )	 
		vel.x = 200;
	if (args->VirtualKey == VirtualKey::Left)
		vel.x = -200;
	if (args->VirtualKey == VirtualKey::Up)
		vel.y = -200;
	if (args->VirtualKey == VirtualKey::Down)
		vel.y = 200;

	if ( ! args->KeyStatus.WasKeyDown )
		keys_down += 1;
}
void Player::ProcessKeyUp(Windows::UI::Core::KeyEventArgs^ args){

	keys_down -= 1;

	if (keys_down == 0)
		vel = (0, 0);
}