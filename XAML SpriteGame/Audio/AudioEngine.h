//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#include "Audio\MediaStreamer.h"
#include "Helpers\BasicMath.h"


namespace AudioEngine
{
	static const int STREAMING_BUFFER_SIZE = 65536;
	static const int MAX_BUFFER_COUNT = 3;

	enum SoundEvent
	{
		Shoot = 0,
		Crash = 1,
		Star = 2,
		Powerup = 3,
		EnemyDead =4,
		 
		LastSoundEvent
	};

	// Make sure this matches the number of entries in the SoundEvent enum above
	static const int SOUND_EVENTS = LastSoundEvent;

	struct SoundEffectData
	{
		SoundEvent                  m_soundEventType;
		IXAudio2SourceVoice*        m_soundEffectSourceVoice;
		XAUDIO2_BUFFER              m_audioBuffer;
		byte*                       m_soundEffectBufferData;
		uint32                      m_soundEffectBufferLength;
		uint32                      m_soundEffectSampleRate;
		bool                        m_soundEffectStarted;
	};

	struct StreamingVoiceContext : public IXAudio2VoiceCallback
	{
		STDMETHOD_(void, OnVoiceProcessingPassStart)(UINT32){}
		STDMETHOD_(void, OnVoiceProcessingPassEnd)(){}
		STDMETHOD_(void, OnStreamEnd)(){}
		STDMETHOD_(void, OnBufferStart)(void*)
		{
			ResetEvent(hBufferEndEvent);
		}
		STDMETHOD_(void, OnBufferEnd)(void* pContext)
		{
			// Trigger the event for the music stream.
			if (pContext == nullptr) {
				SetEvent(hBufferEndEvent);
			}
		}
		STDMETHOD_(void, OnLoopEnd)(void*){}
		STDMETHOD_(void, OnVoiceError)(void*, HRESULT){}

		HANDLE hBufferEndEvent;
		StreamingVoiceContext() : hBufferEndEvent(CreateEventEx(NULL, FALSE, FALSE, NULL))
		{
		}
		virtual ~StreamingVoiceContext()
		{
			hBufferEndEvent = nullptr;
			/*if (hBufferEndEvent != nullptr)
				CloseHandle(hBufferEndEvent);*/
		}
	};

	class Audio;
	class AudioEngineCallbacks : public IXAudio2EngineCallback
	{
	private:
		Audio* m_audio;

	public:
		AudioEngineCallbacks(){};
		void Initialize(Audio* audio);

		// Called by XAudio2 just before an audio processing pass begins.
		void _stdcall OnProcessingPassStart(){};

		// Called just after an audio processing pass ends.
		void  _stdcall OnProcessingPassEnd(){};

		// Called when a critical system error causes XAudio2
		// to be closed and restarted. The error code is given in Error.
		void  _stdcall OnCriticalError(HRESULT Error);
	};

	class Audio
	{
	private:
		bool						m_isMusicEngineStarted;
		bool						m_isSFXEngineStarted;
		IXAudio2*                   m_musicEngine;
		IXAudio2*                   m_soundEffectEngine;

		IXAudio2MasteringVoice*     m_musicMasteringVoice;
		IXAudio2MasteringVoice*     m_soundEffectMasteringVoice;
		IXAudio2SourceVoice*        m_musicSourceVoice;
		StreamingVoiceContext       m_voiceContext;
		byte                        m_audioBuffers[MAX_BUFFER_COUNT][STREAMING_BUFFER_SIZE];
		MediaStreamer               m_musicStreamer;
		uint32                      m_currentBuffer;
		SoundEffectData             m_soundEffects[SOUND_EVENTS];
		XAUDIO2FX_REVERB_PARAMETERS m_reverbParametersSmall;
		IXAudio2SubmixVoice*        m_soundEffectReverbVoiceSmallRoom;
		IXAudio2SubmixVoice*        m_musicReverbVoiceSmallRoom;
		bool                        m_engineExperiencedCriticalError;
		AudioEngineCallbacks        m_musicEngineCallback;
		AudioEngineCallbacks        m_soundEffectEngineCallback;

		void CreateSourceVoice(SoundEvent);
		void CreateReverb(
			IXAudio2* engine,
			IXAudio2MasteringVoice* masteringVoice,
			XAUDIO2FX_REVERB_PARAMETERS* parameters,
			IXAudio2SubmixVoice** newSubmix,
			bool enableEffect
			);

	public:
		bool m_isAudioStarted;

		~Audio();
		void Initialize();
		void CreateResources();
		void ReleaseResources();
		void Start();
		void Render();

		void SuspendMusic();
		void SuspendSFX();

		void StartMusic();
		void StartSFX();

		bool IsMusicStarted();
		bool IsSFXStarted();

		void KillEngine();

		void ReInitializeStreamer(LPCWSTR);

		// This flag can be used to tell when the audio system
		// is experiencing critial errors.
		// XAudio2 gives a critical error when the user unplugs
		// the headphones and a new speaker configuration is generated.
		void SetEngineExperiencedCriticalError()
		{
			m_engineExperiencedCriticalError = true;
		}

		bool HasEngineExperiencedCriticalError()
		{
			return m_engineExperiencedCriticalError;
		}

		void PlaySoundEffect(SoundEvent sound);
		bool IsSoundEffectStarted(SoundEvent sound);
		void StopSoundEffect(SoundEvent sound);
		void SetSoundEffectVolume(SoundEvent sound, float volume);
		void SetAllSoundEffectVolume(float volume);
		void SetMusicVolume(float volume);

		void SetSoundEffectPitch(SoundEvent sound, float pitch);
		void SetSoundEffectFilter(SoundEvent sound, float frequency, float oneOverQ);
		void SetRoomSize(float roomSize, float* wallDistances);
	};
}
