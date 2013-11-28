

#include "pch.h"
#include "HorizontalSliderPlayer.h"

using namespace Windows::System;
using namespace Windows::UI::Core;


HorizontalSliderPlayer::HorizontalSliderPlayer()
{
 
	TargetPos = float2(-1, -1);
	vel = float2(0, 0);
	accel.y = 1000;
}

void HorizontalSliderPlayer::SetTarget(float2 newTarget){
	TargetPos = newTarget;
}

void HorizontalSliderPlayer::Update(float timeDelta)
{
	//vel.y = vel.y + accel.y * timeDelta;

	float2 prevPos = pos;
	float  prevRot = rot;
	if (TargetPos.x != -1) 
		pos = pos + float2(TargetPos.x - pos.x, TargetPos.y - pos.y) * timeDelta;
	else
		pos = pos + vel * timeDelta;

	KeepInBounds();
	UpdateChildren(timeDelta);
	UpdateCollisionGeometry(prevPos, pos, prevRot - rot);
	 
}
void HorizontalSliderPlayer::KeepInBounds()
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
		 
		vel.y = 0;
	}
}


void HorizontalSliderPlayer::ProcessKeyDown(Windows::UI::Xaml::Input::KeyRoutedEventArgs^ args){

	if (args->Key == VirtualKey::Right)
	{
		vel.x = 1000;
	}

	if (args->Key == VirtualKey::Left)
	{
		vel.x = -1000;
	}

	if (args->Key == VirtualKey::Up)
	{

		vel.y = -500;
	}
	if (args->Key == VirtualKey::Down)
	{
		vel.y =  500;
	}

	if (!args->KeyStatus.WasKeyDown)
		keys_down += 1;

	TargetPos = float2(-1, -1);
}
void HorizontalSliderPlayer::ProcessKeyUp(Windows::UI::Xaml::Input::KeyRoutedEventArgs^ args){



	keys_down -= 1;
	if (keys_down == 0)
		vel = (0.f, 0.f);

	if (args->Key == VirtualKey::Space)
	{
		Shoot();
	}
}




void HorizontalSliderPlayer::ProcessPointerPressed(Windows::UI::Input::PointerPoint^ pt)
{

	TargetPos = float2(pt->Position.X, pt->Position.Y);



}
void HorizontalSliderPlayer::ProcessPointerReleased(Windows::UI::Input::PointerPoint^ pt)
{


	TargetPos = float2(-1, -1);
	vel.x = 0;
	rotVel = 0;

}
void HorizontalSliderPlayer::ProcessPointerMoved(Windows::UI::Input::PointerPoint^ pt)
{
	TargetPos = float2(pt->Position.X, pt->Position.Y);

}
