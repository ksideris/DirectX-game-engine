#include "pch.h"
#include "AudioManager.h"

using namespace  AudioEngine;
 
Audio				AudioManager::AudioEngineInstance;
bool				AudioManager::IsMusicStarted;
bool				AudioManager::IsSFXStarted;

int					AudioManager::MusicVolume;
int					AudioManager::SFXVolume;

void AudioManager::Initialize()
{
	MusicVolume = 100;
	SFXVolume = 100;
	AudioEngineInstance.Initialize();
}

void AudioManager::Update()
{
	AudioEngineInstance.Render();
}

void AudioManager::SetGameOverMusic()
{ 
	AudioEngineInstance.~Audio();
	AudioEngineInstance.Initialize();

	AudioEngineInstance.ReInitializeStreamer(L"Assets\\Music\\DST-BeyondTheseForests.wav");

	SetVolume();
	AudioEngineInstance.Start();

	CheckStates();
}

void AudioManager::SetMainMenuMusic()
{ 
	AudioEngineInstance.~Audio();
	AudioEngineInstance.Initialize();

	AudioEngineInstance.ReInitializeStreamer(L"Assets\\Music\\maintheme.wav");

	SetVolume();
	AudioEngineInstance.Start();
	 
}

 

void AudioManager::SetGamePlayMusic(const wchar_t* musicurl)
{

	AudioEngineInstance.~Audio();
	AudioEngineInstance.Initialize();

	AudioEngineInstance.ReInitializeStreamer(  musicurl); 

	SetVolume();
	AudioEngineInstance.Start();

	CheckStates();
 }

void AudioManager::CheckStates()
{
	if (IsMusicStarted)
		AudioEngineInstance.StartMusic();
	else
		AudioEngineInstance.SuspendMusic();

	if (IsSFXStarted)
		AudioEngineInstance.StartSFX();
	else
		AudioEngineInstance.SuspendSFX();
}

void AudioManager::SetMusicVolume(int percent)
{
	MusicVolume = percent;
	AudioEngineInstance.SetMusicVolume((percent / 100.f) * 1.0f);
}

void AudioManager::SetSFXVolume(int percent)
{
	SFXVolume = percent;
	AudioEngineInstance.SetAllSoundEffectVolume((percent / 100.f) * 1.0f);
}

void AudioManager::SetVolume()
{
	SetMusicVolume(MusicVolume);
	SetSFXVolume(SFXVolume);
}