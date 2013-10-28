
#pragma once
#include "GameObject.h"
using namespace Coding4Fun::FallFury::DXCore;
using namespace Coding4Fun::FallFury::DXCore::BasicSprites;


class Player :public GameObject
{
	int keys_down;
public:  
	Player();
	void Update(float timeDelta);
	void KeepInBounds();
	void ProcessKeyDown(Windows::UI::Core::KeyEventArgs^ args);
	void ProcessKeyUp(Windows::UI::Core::KeyEventArgs^ args);
	void ProcessPointer();

};