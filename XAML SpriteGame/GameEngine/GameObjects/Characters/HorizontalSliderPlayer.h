
#pragma once
#include "ControllableCharacter.h"
using namespace DXCore;
using namespace DXCore::BasicSprites;


class HorizontalSliderPlayer :public ControllableCharacter
{
	int keys_down; 
	float2 TargetPos;
public:
	float z;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> spot_texture;
	 
	void Update(float timeDelta);
	void KeepInBounds();

	void SetTarget(float2 newTarget);
	HorizontalSliderPlayer();
	void ProcessKeyDown(Windows::UI::Xaml::Input::KeyRoutedEventArgs^ args);
	void ProcessKeyUp(Windows::UI::Xaml::Input::KeyRoutedEventArgs^ args);
	void ProcessPointerPressed(Windows::UI::Input::PointerPoint^ pt);
	void ProcessPointerReleased(Windows::UI::Input::PointerPoint^ pt);
	void ProcessPointerMoved(Windows::UI::Input::PointerPoint^ pt); 

};