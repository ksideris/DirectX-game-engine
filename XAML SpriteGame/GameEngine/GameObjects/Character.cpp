

#include "pch.h"
#include "Character.h"

using namespace Windows::System;
using namespace Windows::UI::Core;


Character::Character()
{
	health = 100;
	TargetPos = (-1.f, -1.f);

}
void Character::Reset(){
	health = 100;
	TargetPos = (-1.f, -1.f);
	_dead = false;
}
void Character::Shoot()
{

	GlobalData* gdata = GlobalHelper::getData();

	AudioManager::AudioEngineInstance.StopSoundEffect(AudioEngine::SoundEvent::Shoot);
	AudioManager::AudioEngineInstance.PlaySoundEffect(AudioEngine::SoundEvent::Shoot);

	FireBall data(false);
	data.SetPos(pos);
	data.vel = float2(1000.0f* cos(rot), -1000.0f*sin(rot));
	data.SetScale(float2(1.0f, 1.0f));
	data.SetTexture(gdata->m_particle);
	data.setCollisionGeometryForParticle(float2(20, 20), data.GetPos()); 


	gdata->bullets.push_back(data);

	FireBall data2(false);
	data2.SetPos(float2(pos.x - textureSize.Width / 2.f*scale.x, pos.y - textureSize.Height / 2.f*scale.y + 20));
	data2.vel = float2(1000.0f* cos(rot), -1000.0f*sin(rot));
	data2.SetScale(float2(1.0f, 1.0f));
	data2.SetTexture(gdata->m_particle);
	data2.setCollisionGeometryForParticle(float2(20, 20), data2.GetPos()); 

	gdata->bullets.push_back(data2);


	FireBall data3(false);
	data3.SetPos(float2(pos.x - textureSize.Width / 2.f*scale.x, pos.y + textureSize.Height / 2.f*scale.y - 20));
	data3.vel = float2(1000.0f* cos(rot), -1000.0f*sin(rot));
	data3.SetScale(float2(1.0f, 1.0f));
	data3.SetTexture(gdata->m_particle);
	data3.setCollisionGeometryForParticle(float2(20, 20), data3.GetPos()); 


	gdata->bullets.push_back(data3);

}
 
void Character::UpdateChildren(float timeDelta)
{
	GameObject::UpdateChildren(timeDelta);
 
}
ImpactResult Character::ProcessHit(float ImpactFactor)
{
	health -= ImpactFactor;
	if (ImpactFactor>0)
		hit = 10;
	if (health < 0)
	{
		_dead = true;
		AudioManager::AudioEngineInstance.StopSoundEffect(AudioEngine::SoundEvent::EnemyDead);
		AudioManager::AudioEngineInstance.PlaySoundEffect(AudioEngine::SoundEvent::EnemyDead);
		return ImpactResult::bigexplosion;
	}

	AudioManager::AudioEngineInstance.StopSoundEffect(AudioEngine::SoundEvent::Crash);
	AudioManager::AudioEngineInstance.PlaySoundEffect(AudioEngine::SoundEvent::Crash);
	return ImpactResult::hit;
}
void Character::KeepInBounds()
{

	if (pos.x > GlobalHelper::getData()->m_windowBounds.Width - textureSize.Width / 2.0f * scale.x)
	{
		pos.x = GlobalHelper::getData()->m_windowBounds.Width - textureSize.Width / 2.0f * scale.x;
		accel.x = 0;
		vel.x = 0;
	}
	if (pos.y > GlobalHelper::getData()->m_windowBounds.Height - textureSize.Height / 2.0f * scale.y)
	{
		pos.y = GlobalHelper::getData()->m_windowBounds.Height - textureSize.Height / 2.0f * scale.y;
		accel.y = 0;
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
		accel.y = 0;
		vel.y = 0;
	}
}


void Character::SetTarget(float2 newTarget){
	TargetPos = newTarget;
}

void Character::Draw(BasicSprites::SpriteBatch^ m_spriteBatch)
{
	float4 color = float4(1.f, 1.f, 1.f, 1.0f);

	

	if (hit > 0)
	{
			color = float4(1.f, 0.f, 0.f, 1.f);
			hit--;
	}
	for (auto child = children.begin(); child != children.end(); child++)
	{
		child->first->Draw(m_spriteBatch);
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