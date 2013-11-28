#include "pch.h"

#include "FireBall.h"


using namespace BasicSprites;


FireBall::FireBall(bool _isFlipped)
{
	isFlipped = _isFlipped;
	numOfParticles = 100;
	color = float4(1.0f, .4f, .4f, 1.0f);
	for (int i = 0; i < numOfParticles; i++)
	{
		float4 particle;
		if (isFlipped)
			particle.x = RandFloat(1, 50);
		else
			particle.x = RandFloat(-50, 1);
		particle.y = RandFloat(-15.f, 15.f);


		particles.push_back(particle);

	}
}

void  FireBall::setCollisionGeometryForParticle(float2 size, float2 _pos)
{

	setRectangleCollisionGeometry(_pos - float2(10, 10), _pos + float2(10, 10));
}
void FireBall::Update(float timeDelta)
{
	float2 prev_pos = pos;
	pos = pos + vel*timeDelta;

	for (auto particle = particles.begin(); particle != particles.end(); particle++)
	{
		if (isFlipped)
			particle->x = particle->x + RandFloat(1, 5);
		else
			particle->x = particle->x - RandFloat(1, 5);

		if (particle->y < 2 && particle->y > -2)
		{
			particle = particles.erase(particle);

			if (particle == particles.end())
				break;
		}
		if (abs(particle->x) + abs(particle->y * 2) > 50)
			particle->x = 0;

	}
	UpdateCollisionGeometry(prev_pos, pos, 0);
}

void FireBall::Draw(BasicSprites::SpriteBatch^ m_spriteBatch)
{
	for (auto particle = particles.begin(); particle != particles.end(); particle++)
	{
		float2 posoffset = float2(pos.x + particle->x *cos(rot),
			pos.y - particle->x *sin(rot) + particle->y);

		m_spriteBatch->Draw(
			_texture.Get(),
			posoffset,
			PositionUnits::DIPs,
			scale*float2(20.0f, 20.0f)* abs(abs(particle->x) - 49.f) / 50.f,
			SizeUnits::DIPs,
			color,
			0.0f,
			BlendMode::Additive
			);

	}
}

float2 FireBall::GetTopLeft(){
	return float2(pos.x - scale.x, pos.y - scale.y);
}
float2 FireBall::GetBottomRight(){
	return float2(pos.x + scale.x, pos.y + scale.y);
}

void FireBall::UpdateCollisionGeometry(float2 prevPos, float2 pos, float rot)
{
	translateCollisionGeometry(pos - prevPos);

}


ImpactResult FireBall::ProcessHit(float ImpactFactor)
{
	_dead = true;
	return ImpactResult::explosion;
}