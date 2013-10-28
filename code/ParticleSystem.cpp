#include "pch.h"

#include "ParticleSystem.h"


using namespace BasicSprites;


ParticleSystem::ParticleSystem()
{
	for (int i = 0; i < numOfParticles; i++)
	{
		float4 particle; 
		particle.r = 1.f;
		particle.g =  1.2f;
		particle.b =  .00f;
		particle.a =  1.0f;

		particles.push_back(particle);

	}
}
void ParticleSystem::Update(float timeDelta)
{
	pos = pos + vel * timeDelta;

 
}
 
void ParticleSystem::Draw(BasicSprites::SpriteBatch^ m_spriteBatch)
{  
	int i = -200;

	for (auto particle : particles)
	{ 
		m_spriteBatch->Draw(
			_texture.Get(),
			float2(  pos.x, pos.y  ),
			PositionUnits::DIPs,
			float2(22.0f, 22.0f),
			SizeUnits::DIPs,
			float4(particle.r, particle.g, particle.b, particle.a),
			0.0f,
			BlendMode::Additive
			); 

	}
	for (auto particle:particles )
	{
		float x = i;
		float y = sqrt(900 - x*x);
		m_spriteBatch->Draw(
			_texture.Get(),
			float2(x + pos.x, y + pos.y),
			PositionUnits::DIPs,
			float2(12.0f, 12.0f),
			SizeUnits::DIPs,
			float4(particle.r, particle.g, particle.b, particle.a),
			0.0f,
			BlendMode::Additive
			);  
		i +=  RandFloat(1,5);

	}
	  i = -200;
	for (auto particle : particles)
	{
		float x = i;
		float y = -sqrt(900 - x*x);
		m_spriteBatch->Draw(
			_texture.Get(),
			float2(x + pos.x, y + pos.y),
			PositionUnits::DIPs,
			float2(12.0f, 12.0f),
			SizeUnits::DIPs,
			float4(particle.r, particle.g, particle.b, particle.a),
			0.0f,
			BlendMode::Additive
			);
		i += 2 + RandFloat(1, 5);

	}
}

float ParticleSystem::RandFloat(float min, float max)
{
	return (static_cast<float>(rand() % RAND_MAX) / static_cast<float>(RAND_MAX)) * (max - min) + min;
}

