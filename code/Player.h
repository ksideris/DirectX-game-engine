

#include "DirectXBase.h"
#include "BasicSprites.h"
#include "GameObject.h"

class Player :public GameObject
{
	int keys_down = 0;
public:  
	void Update(float timeDelta);
	void KeepInBounds();
	void ProcessKeyDown(Windows::UI::Core::KeyEventArgs^ args);
	void ProcessKeyUp(Windows::UI::Core::KeyEventArgs^ args);
	void ProcessPointer();

};