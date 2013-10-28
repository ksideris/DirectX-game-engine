#include "DirectXBase.h"
#include "BasicSprites.h"
#include "Sprite.h"

class ParticleSystem :public Sprite
{
protected:

	int numOfParticles = 100; 
	std::vector<float4> particles;
public: 
	
	float2 vel;  
	ParticleSystem();
	void Update(float timeDelta);
	void Draw(BasicSprites::SpriteBatch^ m_spriteBatch);


	float RandFloat(float min, float max);

};