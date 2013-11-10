#pragma once
#include "pch.h"
#include "Audio\AudioEngine.h" 

namespace Coding4Fun
{
	namespace FallFury
	{
		namespace Audio
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
					static void SetGamePlayMusic( );
		
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
	}
}
