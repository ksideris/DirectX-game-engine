#include "pch.h"
#include "RocketFire.h"


using namespace BasicSprites;


RocketFire::RocketFire()
{
	numOfParticles = 100;
	for (int i = 0; i < numOfParticles; i++)
	{
		float4 particle;
		particle.x = RandFloat(-100, 1);
		particle.y = RandFloat(-25.f,25.f);


		particles.push_back(particle);

	}
}
void RocketFire::Update(float timeDelta)
{
	 
	for (auto particle = particles.begin(); particle != particles.end(); particle++)
	{
		particle->x = particle->x - RandFloat(1, 5);
		  if (particle->y < 2 && particle->y > -2)
		{
			particle = particles.erase(particle);

			if (particle == particles.end())
				break;
		} 

		 
	 
		if (abs(particle->x) + abs(particle->y * 2) > 100)
			particle->x = 0;
	}
}

void RocketFire::Draw(BasicSprites::SpriteBatch^ m_spriteBatch)
{
	for (auto particle = particles.begin(); particle != particles.end(); particle++)
	{ 
		float2 posoffset = float2(pos.x +  particle->x *cos(rot),
			pos.y - particle->x *sin(rot) + particle->y);
	 
		m_spriteBatch->Draw(
			_texture.Get(),
			posoffset,
			PositionUnits::DIPs,
			float2(30.0f, 30.0f)*(-particle->x-101.f) / 50.f,
			SizeUnits::DIPs,
			float4(.6f, .4f, .4f, 1.0f),
			0.0f,
			BlendMode::Additive
			);

	}

}