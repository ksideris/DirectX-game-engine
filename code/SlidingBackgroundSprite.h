

#include "Sprite.h"


class SlidingBackgroundSprite : public Sprite
{
	float vel = 10;
	float2 second_pos;

public:
	void InitSliding();
	void Update(float timeDelta);
	void Draw(BasicSprites::SpriteBatch^ m_spriteBatch);
};