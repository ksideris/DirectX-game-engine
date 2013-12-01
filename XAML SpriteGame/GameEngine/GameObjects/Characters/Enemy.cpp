#include "pch.h"
#include "Enemy.h"


Enemy::Enemy(EnemyMovement _moveType)
{
	GameObject::GameObject();
	movementType = _moveType;

	vel = float2(0.f, 0.f);
	health = 100.f;
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
void Enemy::SetTarget(float2 newTarget){  //Why does it not work with the parent??
	TargetPos = newTarget;
}
void Enemy::Update(float timeDelta)
{

	if (dist(TargetPos, pos) < 5)
	{

		Shoot();
		switch (movementType)
		{
		case EnemyMovement::RANDOM:
			{
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

	UpdateChildren(timeDelta);
	UpdateCollisionGeometry(prevPos, pos, prevRot - rot);

}
//ImpactResult Enemy::ProcessHit(float ImpactFactor)
//{
//	health -= ImpactFactor;
//	if (ImpactFactor>0)
//		hit = 10;
//	if (health < 0)
//	{
//		_dead = true;
//		AudioManager::AudioEngineInstance.StopSoundEffect(AudioEngine::SoundEvent::EnemyDead);
//		AudioManager::AudioEngineInstance.PlaySoundEffect(AudioEngine::SoundEvent::EnemyDead);
//		return ImpactResult::bigexplosion;
//	}
//
//	AudioManager::AudioEngineInstance.StopSoundEffect(AudioEngine::SoundEvent::Crash);
//	AudioManager::AudioEngineInstance.PlaySoundEffect(AudioEngine::SoundEvent::Crash);
//	return ImpactResult::hit;
//}
float Enemy::GetImpactSize()
{
	return 10.f;
}