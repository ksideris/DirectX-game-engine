

#include "Sprite.h"


class BackgroundSprite : public Sprite
{
	float vel = 10;
public:
	void Update(float timeDelta);
};