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
		particle.y = RandFloat(-30.f,30.f);


		particles.push_back(particle);

	}
}
void RocketFire::Update(float timeDelta)
{
	

	//pos = pos + vel * timeDelta;
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
		float2 posoffset = float2(ParentObject->pos.x - (ParentObject->textureSize.Width / 2.0*ParentObject->scale -30- particle->x)*cos(ParentObject->rot),
			ParentObject->pos.y + (ParentObject->textureSize.Height / 2.0*ParentObject->scale - 30 - particle->x)*sin(ParentObject->rot) - particle->y );

		m_spriteBatch->Draw(
			_texture.Get(),
			posoffset,
			PositionUnits::DIPs,
			float2(12.0f, 12.0f),
			SizeUnits::DIPs,
			float4(.6, .4, .4, 1.0f),
			0.0f,
			BlendMode::Additive
			);

	}

}