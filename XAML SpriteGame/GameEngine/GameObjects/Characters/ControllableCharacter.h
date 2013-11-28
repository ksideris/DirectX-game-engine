
#pragma once
#include "../Character.h"
using namespace DXCore;
using namespace DXCore::BasicSprites;


class ControllableCharacter :public Character
{
	int keys_down; 
public:
 

	virtual void ProcessKeyDown(Windows::UI::Xaml::Input::KeyRoutedEventArgs^ args) = 0;
	virtual void ProcessKeyUp(Windows::UI::Xaml::Input::KeyRoutedEventArgs^ args) = 0;
	virtual void ProcessPointerPressed(Windows::UI::Input::PointerPoint^ pt) = 0;
	virtual void ProcessPointerReleased(Windows::UI::Input::PointerPoint^ pt) = 0;
	virtual void ProcessPointerMoved(Windows::UI::Input::PointerPoint^ pt) = 0;

};