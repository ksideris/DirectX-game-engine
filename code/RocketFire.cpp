#include "pch.h"
#include "RocketFire.h"


using namespace BasicSprites;


RocketFire::RocketFire()
{
	for (int i = 0; i < numOfParticles; i++)
	{
		float4 particle;
		particle.x = RandFloat(-100, 1);
		particle.y = RandFloat(-30, 30);
		particle.b = 1.00f;
		particle.a = 1.0f;

		particles.push_back(particle);

	}
}
void RocketFire::Update(float timeDelta)
{
	//pos = pos + vel * timeDelta;
	for (auto particle = particles.begin(); particle != particles.end(); particle++)
	{
		particle->x = particle->x - RandFloat(1,5);
		if (abs(particle->x) + abs(particle->y*2) > 100)
			particle->x = 0;
	}
}

void RocketFire::Draw(BasicSprites::SpriteBatch^ m_spriteBatch)
{ 
	for (auto particle : particles)
	{ 
		m_spriteBatch->Draw(
			_texture.Get(),
			float2(particle.x+pos.x, pos.y + particle.y),
			PositionUnits::DIPs,
			float2(12.0f, 12.0f),
			SizeUnits::DIPs,
			float4(.3, .4, .4, particle.a),
			0.0f,
			BlendMode::Additive
			); 

	} 
	 
}