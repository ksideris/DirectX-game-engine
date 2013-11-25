#include "pch.h"
#include "Enemy.h"


Enemy::Enemy(EnemyMovement _moveType)
{
	movementType = _moveType; 
	lastFired = 0.f;
	vel = float2(0.f, 0.f);
	health = 100.f;
	timeAlive = 0;
	TargetPos = (-1, -1);
}
void Enemy::Shoot()
{

	FireBall data(true);
	data.SetPos(pos);
	data.color = float4(0.0f, 1.f, 0.f, 1.0f);
	data.vel = float2(-1000.0f* cos(rot), -1000.0f*sin(rot));
	data.SetScale(float2(1.0f, 1.0f));
	data.SetTexture(_projectile);
	data.setCollisionGeometryForParticle(float2(20, 20), data.GetPos());
	data.SetWindowSize(_windowRect);
	bullets.push_back(data);

}
void Enemy::SetTarget(float2 newTarget){
	TargetPos = newTarget;
}
void Enemy::Update(float timeDelta)
{

	timeAlive += timeDelta;
	if (dist(TargetPos, pos) < 5)
	{

		Shoot();
		switch (movementType)
		{
		case EnemyMovement::RANDOM:
			{
				if (dist(TargetPos, pos) < 5)
					TargetPos = float2(RandFloat(_windowRect.Width / 2, _windowRect.Width - textureSize.Width), RandFloat(textureSize.Height, _windowRect.Height - textureSize.Height));
				break;
								  }
		case EnemyMovement::UP_DOWN:
			{

				TargetPos = float2(pos.x, RandFloat(0, _windowRect.Height));
				break;
								   }
		}
	}

	float2 prevPos = pos;
	float  prevRot = rot;
	if (TargetPos.x != -1)
		pos = pos + float2(TargetPos.x - pos.x, TargetPos.y - pos.y) *2.f* timeDelta;
	else
		pos = pos + vel * timeDelta;

	KeepInBounds();
	UpdateChildren(timeDelta);
	UpdateCollisionGeometry(prevPos, pos, prevRot - rot);

}
void Enemy::KeepInBounds()
{

	if (pos.x > _windowRect.Width - textureSize.Width / 2.0f * scale.x)
	{
		pos.x = _windowRect.Width - textureSize.Width / 2.0f * scale.x;
		accel.x = 0;
		vel.x = 0;
	}
	if (pos.y > _windowRect.Height - textureSize.Height / 2.0f * scale.y)
	{
		pos.y = _windowRect.Height - textureSize.Height / 2.0f * scale.y;

		vel.y = 0;
	}
	if (pos.x < textureSize.Width / 2.0f * scale.x)
	{
		pos.x = textureSize.Width / 2.0f * scale.x;
		accel.x = 0;
		vel.x = 0;
	}
	if (pos.y < textureSize.Height / 2.0f * scale.y)
	{
		pos.y = textureSize.Height / 2.0f * scale.y;

		vel.y = 0;
	}
}
void Enemy::ProcessHit(float ImpactFactor)
{
	health -= ImpactFactor;
	hit = 10;
	if (health < 0)
		dead = true;
}
void Enemy::Draw(BasicSprites::SpriteBatch^ m_spriteBatch)
{
	float4 color = float4(1.f, 1.f, 1.f, 1.0f);

	for (auto particle = bullets.begin(); particle != bullets.end(); particle++)
	{
		particle->Draw(m_spriteBatch);
	}
	for (auto child = children.begin(); child != children.end(); child++)
	{
		child->first->Draw(m_spriteBatch);
	}
	if (hit>0)
	{
		color = float4(1.f, 0.f, 0.f, 1.0f);
		hit--;
	} 

		m_spriteBatch->Draw(
			_texture.Get(),
			pos,
			PositionUnits::DIPs,
			float2(1.0f, 1.0f) * scale,
			SizeUnits::Normalized,
			color,
			rot

			);
	


}