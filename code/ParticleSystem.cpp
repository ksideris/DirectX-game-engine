#include "pch.h"

#include "ParticleSystem.h"


using namespace BasicSprites;


void ParticleSystem::Update(float timeDelta)
{


	pos = pos + vel * timeDelta;


}
 
void ParticleSystem::Draw(BasicSprites::SpriteBatch^ m_spriteBatch)
{
	for (int i = 0; i < numOfParticles; i++)
	{
		float alpha = 1.0f;
		m_spriteBatch->Draw(
			_texture.Get(),
			pos,
			PositionUnits::DIPs,
			float2(32.0f, 32.0f),
			SizeUnits::DIPs,
			float4(0.1f, 0.02f, 0.0f, alpha),
			0.0f,
			BlendMode::Additive
			);

	}
}