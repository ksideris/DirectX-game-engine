#pragma once
#include "pch.h"
#include "Audio\AudioEngine.h" 


namespace AudioEngine
{

	class AudioManager
	{
	public:
		static void Initialize();
		static void Update();

		static void SetGameOverMusic();
		static void SetMainMenuMusic();
		static void SetGameWonMusic();
		static void CheckStates();
		static void SetGamePlayMusic(const wchar_t* musicurl);

		static void SetMusicVolume(int percent);
		static void SetSFXVolume(int percent);

		static Audio				AudioEngineInstance;

		static bool IsMusicStarted;
		static bool IsSFXStarted;

		static int MusicVolume;
		static int SFXVolume;

	private:
		static void SetVolume();

	};
}