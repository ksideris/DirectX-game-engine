
#pragma once 
#include "ControllableCharacter.h"
using namespace DXCore;
using namespace DXCore::BasicSprites;


class DirectionalPlayer : public ControllableCharacter
{
	int keys_down; 
	float2 TargetPos;

	float z;
public:

	Microsoft::WRL::ComPtr<ID3D11Texture2D> spot_texture;

	DirectionalPlayer();


	void Update(float timeDelta);

	void KeepInBounds();


	void ProcessKeyDown(Windows::UI::Xaml::Input::KeyRoutedEventArgs^ args);
	void ProcessKeyUp(Windows::UI::Xaml::Input::KeyRoutedEventArgs^ args);
	void ProcessPointerPressed(Windows::UI::Input::PointerPoint^ pt);
	void ProcessPointerReleased(Windows::UI::Input::PointerPoint^ pt);
	void ProcessPointerMoved(Windows::UI::Input::PointerPoint^ pt); 

};