#include "DirectXBase.h"
#include "BasicSprites.h"
#include "Sprite.h"

class ParticleSystem :public Sprite
{
	int numOfParticles = 100;
public: 
	float2 vel; 

	void Update(float timeDelta);
	void Draw(BasicSprites::SpriteBatch^ m_spriteBatch);



};