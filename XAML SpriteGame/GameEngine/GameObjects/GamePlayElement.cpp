#include "pch.h"

#include "GamePlayElement.h"

void GamePlayElement::Update(float timeDelta)
{
	GameObject::Update(timeDelta);
}

float GamePlayElement::GetImpactSize()
{
	return 0;
}
ImpactResult GamePlayElement::ProcessHit(float ImpactFactor)
{
	_dead = true;

	if (type == GamePlayType::RING)
	{
		AudioManager::AudioEngineInstance.StopSoundEffect(AudioEngine::SoundEvent::Star);
		AudioManager::AudioEngineInstance.PlaySoundEffect(AudioEngine::SoundEvent::Star);
		return ImpactResult::score;
	}
	else if (type == GamePlayType::FINISHLINE)
	{
		_dead = false;
		AudioManager::AudioEngineInstance.StopSoundEffect(AudioEngine::SoundEvent::Powerup);
		AudioManager::AudioEngineInstance.PlaySoundEffect(AudioEngine::SoundEvent::Powerup);
		return ImpactResult::finish;

	}

	else if (type == GamePlayType::HEALTH)
	{ 
		AudioManager::AudioEngineInstance.StopSoundEffect(AudioEngine::SoundEvent::Powerup);
		AudioManager::AudioEngineInstance.PlaySoundEffect(AudioEngine::SoundEvent::Powerup);
		return ImpactResult::health;

	}

	else if (type == GamePlayType::UPGRADE)
	{ 
		AudioManager::AudioEngineInstance.StopSoundEffect(AudioEngine::SoundEvent::Powerup);
		AudioManager::AudioEngineInstance.PlaySoundEffect(AudioEngine::SoundEvent::Powerup);
		return ImpactResult::weapon_upgrade;

	}

	return ImpactResult::death;
}