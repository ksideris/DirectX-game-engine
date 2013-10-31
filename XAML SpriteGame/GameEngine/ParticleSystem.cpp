#include "pch.h"

#include "ParticleSystem.h"


using namespace BasicSprites;


ParticleSystem::ParticleSystem()
{

	size = float2(20,20);
	numOfParticles = 100;
	for (int i = 0; i < numOfParticles; i++)
	{
		float4 particle; 
		particle.r = 1.f;
		particle.g =  .3f;
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

	 	m_spriteBatch->Draw(
			_texture.Get(),
			float2(  pos.x, pos.y  ),
			PositionUnits::DIPs,
			float2(22.0f, 22.0f),
			SizeUnits::DIPs,
			float4(particles[0].r, particles[0].g, particles[0].b, particles[0].a),
			0.0f,
			BlendMode::Additive
			); 

	 
	for (auto particle:particles )
	{
		float x = i;
		float y = sqrt(size.x*size.y - x*x);
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
		float y = -sqrt(size.x*size.y - x*x);
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
float2 ParticleSystem::GetTopLeft(){
	return float2(pos.x - size.x, pos.y - size.y);
}
float2 ParticleSystem::GetBottomRight(){
	return float2(pos.x + size.x, pos.y + size.y);
}

bool ParticleSystem::IsColliding(GameObject  otherObj)
{
	float2 tl1 = GetTopLeft();
	float2 tl2 = otherObj.GetTopLeft();
	float2 br1 = GetBottomRight();
	float2 br2 = otherObj.GetBottomRight();

	if (br1.y  < tl2.y) return(0);
	if (tl1.y  > br2.y) return(0);

	if (br1.x < tl2.x) return(0);
	if (tl1.x > br2.x) return(0);

	return 1; 

}