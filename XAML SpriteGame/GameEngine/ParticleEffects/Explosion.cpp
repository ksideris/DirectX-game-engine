#include "pch.h"
#include "Explosion.h"

Explosion::Explosion()
{
	numOfParticles = 100;


	scale = float2(30.f, 30.f);
	color = float4(1.0f, .4f, .4f, 1.0f);
	for (int i = 0; i < numOfParticles; i++)
	{
		float4 particle;
		particle.x = 0;

		particle.z = 2 * i* M_PI / numOfParticles;
		particles.push_back(particle);
	}
}
Explosion::Explosion(float2 _size, float4 _color)
{
	numOfParticles = 100;
	color = _color;
	scale = _size;
	for (int i = 0; i < numOfParticles; i++)
	{
		float4 particle;
		particle.x = 0;
		particle.y = 0;
		particle.z = 2 * i* M_PI / numOfParticles;
		particles.push_back(particle);

	}
}
void Explosion::Update(float timeDelta)
{


	for (auto particle = particles.begin(); particle != particles.end(); particle++)
	{

		particle->x = particle->x + RandFloat(5, 10);

	}

	GameObject::Update(timeDelta);
}

void Explosion::Draw(BasicSprites::SpriteBatch^ m_spriteBatch)
{
	for (auto particle = particles.begin(); particle != particles.end(); particle++)
	{
		float2 posoffset = float2(pos.x + particle->x*cos(particle->z), pos.y + particle->x * sin(particle->z));

		m_spriteBatch->Draw(
			_texture.Get(),
			posoffset,
			PositionUnits::DIPs,
			scale* abs(abs(particle->x) - 2200.f) / 2200.f,
			SizeUnits::DIPs,
			color,
			0.4f,
			BlendMode::Additive
			);

	}
}