#include "pch.h"

#include "FireBall.h"


using namespace BasicSprites;


FireBall::FireBall()
{

	size = float2(20, 20);

	numOfParticles = 10;
	for (int i = 0; i < numOfParticles; i++)
	{
		float4 particle;
		particle.r = 1.f;
		particle.g = .3f;
		particle.b = .00f;
		particle.a = 1.0f;

		particles.push_back(particle);

	}
}

void  FireBall::setCollisionGeometryForParticle(float2 size,float2 _pos)
{

	setRectangleCollisionGeometry(_pos - float2(10, 10), _pos + float2(10, 10));
}
void FireBall::Update(float timeDelta)
{
	float2   prevPos = pos;
	pos = pos + vel * timeDelta;


	UpdateCollisionGeometry(prevPos, pos, 0);
}

void FireBall::Draw(BasicSprites::SpriteBatch^ m_spriteBatch)
{
	float i = -20;

	m_spriteBatch->Draw(
		_texture.Get(),
		float2(pos.x, pos.y),
		PositionUnits::DIPs,
		float2(22.0f, 22.0f),
		SizeUnits::DIPs,
		float4(particles[0].r, particles[0].g, particles[0].b, particles[0].a),
		0.0f,
		BlendMode::Additive
		);


	for (auto particle : particles)
	{
		float x = i ;
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
		i += RandFloat(1, 5);

	}
	i = -20;
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
 
float2 FireBall::GetTopLeft(){
	return float2(pos.x - size.x, pos.y - size.y);
}
float2 FireBall::GetBottomRight(){
	return float2(pos.x + size.x, pos.y + size.y);
}

void FireBall::UpdateCollisionGeometry(float2 prevPos, float2 pos, float rot)
{
	translateCollisionGeometry(pos - prevPos);
	//rotateCollisionGeometry(rot, pos);

}