//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#include "pch.h"
#include "DXCore\DirectXLinker.h"
#include "AudioEngine.h"

using namespace DXCore;
using namespace AudioEngine;

void AudioEngineCallbacks::Initialize(Audio* audio)
{
    m_audio = audio;
}

// Called when a critical system error causes XAudio2
// to be closed and restarted. The error code is given in Error.
void  _stdcall AudioEngineCallbacks::OnCriticalError(HRESULT Error)
{
    m_audio->SetEngineExperiencedCriticalError();
}

Audio::~Audio()
{
	KillEngine();
}

void Audio::Initialize()
{
    m_isAudioStarted = false;
    m_musicEngine = nullptr;
    m_soundEffectEngine = nullptr;
    m_musicMasteringVoice = nullptr;
    m_soundEffectMasteringVoice = nullptr;
    m_musicSourceVoice = nullptr;

    for (int i = 0; i < ARRAYSIZE(m_soundEffects); i++)
    {
        m_soundEffects[i].m_soundEffectBufferData = nullptr;
        m_soundEffects[i].m_soundEffectSourceVoice = nullptr;
        m_soundEffects[i].m_soundEffectStarted = false;
        ZeroMemory(&m_soundEffects[i].m_audioBuffer, sizeof(m_soundEffects[i].m_audioBuffer));
    }
    ZeroMemory(m_audioBuffers, sizeof(m_audioBuffers));

	CreateResources();
}

void Audio::SetRoomSize(float roomSize, float* wallDistances)
{
    // Maximum "outdoors" is a room size of 1000.0f
    float normalizedRoomSize = roomSize  / 1000.0f;

    float outputMatrix[4] = {0, 0, 0, 0};
    float leftRatio = wallDistances[2] / (wallDistances[2] + wallDistances[3]);
    if ((wallDistances[2] + wallDistances[3]) == 0)
    {
        leftRatio = 0.5f;
    }
    float width = abs((wallDistances[3] + wallDistances[5] + wallDistances[7]) / 3.0f - (wallDistances[2] + wallDistances[4] + wallDistances[6]) / 3.0f);
    float height = abs(wallDistances[1] - wallDistances[0]);
    float widthNormalized = width / max(width, height);
    if (width == 0)
    {
        widthNormalized = 1.0f;
    }

    // As widthNormalized approaches 0, the room is taller than wide.
    // This means that the late-field reverb should be non-localized across both speakers.
    leftRatio -= .5f;
    leftRatio *= widthNormalized;
    leftRatio += .5f;

    // Near-field reverb with the speaker closest to the wall getting more reverb
    outputMatrix[0] = (1.0f - leftRatio)* normalizedRoomSize;
    outputMatrix[1] = 0;
    outputMatrix[2] = 0;
    outputMatrix[3] = leftRatio * normalizedRoomSize;
    ThrowIfFailed(
        m_soundEffectReverbVoiceSmallRoom->SetOutputMatrix(m_soundEffectMasteringVoice, 2, 2, outputMatrix)
        );

    outputMatrix[0] = (1.0f - leftRatio) * normalizedRoomSize;
    outputMatrix[1] = 0;
    outputMatrix[2] = 0;
    outputMatrix[3]= leftRatio * normalizedRoomSize;
    ThrowIfFailed(
        m_musicReverbVoiceSmallRoom->SetOutputMatrix(m_musicMasteringVoice, 2, 2, outputMatrix)
        );
}

void Audio::ReInitializeStreamer(LPCWSTR location)
{
	m_musicStreamer.Initialize(location);

	XAudio2Create(&m_musicEngine);
	//ThrowIfFailed(XAudio2Create(&m_musicEngine));

#if defined(_DEBUG)
    XAUDIO2_DEBUG_CONFIGURATION debugConfig = {0};
    debugConfig.BreakMask = XAUDIO2_LOG_ERRORS;
    debugConfig.TraceMask = XAUDIO2_LOG_ERRORS;
    m_musicEngine->SetDebugConfiguration(&debugConfig);
#endif

    m_musicEngineCallback.Initialize(this);
    m_musicEngine->RegisterForCallbacks(&m_musicEngineCallback);

	HRESULT hrs = m_musicEngine->CreateMasteringVoice(&m_musicMasteringVoice, 2, 48000, 0, nullptr, nullptr, AudioCategory_GameMedia);

	if (FAILED(hrs))
	{
		ReInitializeStreamer(location);
	}
	//ThrowIfFailed(m_musicEngine->CreateMasteringVoice(&m_musicMasteringVoice, 2, 48000, 0, nullptr, nullptr, AudioCategory_GameMedia));

    CreateReverb(
        m_musicEngine,
        m_musicMasteringVoice,
        &m_reverbParametersSmall,
        &m_musicReverbVoiceSmallRoom,
        true
        );

	XAUDIO2_SEND_DESCRIPTOR descriptors[2];
	descriptors[0].pOutputVoice = m_musicMasteringVoice;
	descriptors[0].Flags = 0;
	descriptors[1].pOutputVoice = m_musicReverbVoiceSmallRoom;
	descriptors[1].Flags = 0;

	XAUDIO2_VOICE_SENDS sends = {0};
	sends.SendCount = 2;
	sends.pSends = descriptors;

	WAVEFORMATEX& waveFormat = m_musicStreamer.GetOutputWaveFormatEx();

	ThrowIfFailed(m_musicEngine->CreateSourceVoice(&m_musicSourceVoice, &waveFormat, 0, 1.0f, &m_voiceContext, &sends, nullptr));

    ThrowIfFailed(m_musicMasteringVoice->SetVolume(0.4f));

}

void Audio::CreateResources()
{
    try
    {
        ThrowIfFailed(XAudio2Create(&m_soundEffectEngine));

        m_soundEffectEngineCallback.Initialize(this);
        m_soundEffectEngine->RegisterForCallbacks(&m_soundEffectEngineCallback);

        ThrowIfFailed(
            m_soundEffectEngine->CreateMasteringVoice(&m_soundEffectMasteringVoice, 2, 48000, 0, nullptr, nullptr, AudioCategory_GameEffects)
            );

        CreateReverb(m_soundEffectEngine, m_soundEffectMasteringVoice, &m_reverbParametersSmall, &m_soundEffectReverbVoiceSmallRoom, true);

		CreateSourceVoice(Shoot);
		CreateSourceVoice(Crash);
		CreateSourceVoice(Star);
		CreateSourceVoice(Powerup);
		CreateSourceVoice(EnemyDead);
	 
    }
    catch (...)
    {
        m_engineExperiencedCriticalError = true;
    }
}

void Audio::CreateReverb(IXAudio2* engine, IXAudio2MasteringVoice* masteringVoice, XAUDIO2FX_REVERB_PARAMETERS* parameters, IXAudio2SubmixVoice** newSubmix, bool enableEffect)
{
    XAUDIO2_EFFECT_DESCRIPTOR soundEffectdescriptor;
    XAUDIO2_EFFECT_CHAIN soundEffectChain;
    Microsoft::WRL::ComPtr<IUnknown> soundEffectXAPO;

    ThrowIfFailed(
        XAudio2CreateReverb(&soundEffectXAPO)
        );

    soundEffectdescriptor.InitialState = false;
    soundEffectdescriptor.OutputChannels = 2;
    soundEffectdescriptor.pEffect = soundEffectXAPO.Get();
    soundEffectChain.EffectCount = 1;
    soundEffectChain.pEffectDescriptors = &soundEffectdescriptor;

    ThrowIfFailed(
        engine->CreateSubmixVoice(newSubmix, 2, 48000, 0, 0, nullptr, &soundEffectChain)
        );

    ThrowIfFailed(
        (*newSubmix)->SetEffectParameters(0, parameters, sizeof(m_reverbParametersSmall))
        );

    if (enableEffect)
    {
        ThrowIfFailed(
            (*newSubmix)->EnableEffect(0)
            );
    }

    ThrowIfFailed(
        (*newSubmix)->SetVolume (1.0f)
        );

    float outputMatrix[4] = {0, 0, 0, 0};
    ThrowIfFailed(
        (*newSubmix)->SetOutputMatrix(masteringVoice, 2, 2, outputMatrix)
        );
}

void Audio::CreateSourceVoice(SoundEvent sound)
{
    // Load all data for each sound effect into a single in-memory buffer
    MediaStreamer soundEffectStream;
    switch (sound)
    {
         
	case Crash: soundEffectStream.Initialize(L"Assets\\Sounds\\Crash.wav"); break;
	case Shoot: soundEffectStream.Initialize(L"Assets\\Sounds\\Shoot.wav"); break;
	case Star: soundEffectStream.Initialize(L"Assets\\Sounds\\Coin1.wav"); break;
	case Powerup: soundEffectStream.Initialize(L"Assets\\Sounds\\GenericPowerup.wav"); break;
	case EnemyDead: soundEffectStream.Initialize(L"Assets\\Sounds\\EnemyDead1.wav"); break;
    }

    m_soundEffects[sound].m_soundEventType = sound;

    uint32 bufferLength = soundEffectStream.GetMaxStreamLengthInBytes();
    m_soundEffects[sound].m_soundEffectBufferData = new byte[bufferLength];
    soundEffectStream.ReadAll(m_soundEffects[sound].m_soundEffectBufferData, bufferLength, &m_soundEffects[sound].m_soundEffectBufferLength);

    XAUDIO2_SEND_DESCRIPTOR descriptors[2];
    descriptors[0].pOutputVoice = m_soundEffectMasteringVoice;
    descriptors[0].Flags = 0;
    descriptors[1].pOutputVoice = m_soundEffectReverbVoiceSmallRoom;
    descriptors[1].Flags = 0;

    XAUDIO2_VOICE_SENDS sends = {0};
    sends.SendCount = 2;
    sends.pSends = descriptors;

    // The rolling sound can have pitch shifting and a low-pass filter.

        ThrowIfFailed(
            m_soundEffectEngine->CreateSourceVoice(
                &m_soundEffects[sound].m_soundEffectSourceVoice,
                &(soundEffectStream.GetOutputWaveFormatEx()),
                0,
                1.0f,
                &m_voiceContext,
                &sends)
            );

    m_soundEffects[sound].m_soundEffectSampleRate = soundEffectStream.GetOutputWaveFormatEx().nSamplesPerSec;

    // Queue in-memory buffer for playback
    ZeroMemory(&m_soundEffects[sound].m_audioBuffer, sizeof(m_soundEffects[sound].m_audioBuffer));

    m_soundEffects[sound].m_audioBuffer.AudioBytes = m_soundEffects[sound].m_soundEffectBufferLength;
    m_soundEffects[sound].m_audioBuffer.pAudioData = m_soundEffects[sound].m_soundEffectBufferData;
    m_soundEffects[sound].m_audioBuffer.pContext = &m_soundEffects[sound];
    m_soundEffects[sound].m_audioBuffer.Flags = XAUDIO2_END_OF_STREAM;

    m_soundEffects[sound].m_audioBuffer.LoopCount = 0;

    m_soundEffects[sound].m_soundEffectSourceVoice->SetVolume(0.4f);

    ThrowIfFailed(
        m_soundEffects[sound].m_soundEffectSourceVoice->SubmitSourceBuffer(&m_soundEffects[sound].m_audioBuffer)
        );
}

void Audio::ReleaseResources()
{
    if (m_musicSourceVoice != nullptr)
    {
        m_musicSourceVoice->DestroyVoice();
    }
    if (m_soundEffectReverbVoiceSmallRoom != nullptr)
    {
        m_soundEffectReverbVoiceSmallRoom->DestroyVoice();
    }
    if (m_musicReverbVoiceSmallRoom != nullptr)
    {
        m_musicReverbVoiceSmallRoom->DestroyVoice();
    }
    for (int i = 0; i < ARRAYSIZE(m_soundEffects); i++)
    {
        if (m_soundEffects[i].m_soundEffectSourceVoice != nullptr)
        {
            m_soundEffects[i].m_soundEffectSourceVoice->DestroyVoice();
        }
        m_soundEffects[i].m_soundEffectSourceVoice = nullptr;

		if (m_soundEffects[i].m_soundEffectBufferData != nullptr)
		{
			delete[] m_soundEffects[i].m_soundEffectBufferData;
		}
		m_soundEffects[i].m_soundEffectBufferData = nullptr;
    }
    if (m_musicMasteringVoice != nullptr)
    {
            m_musicMasteringVoice->DestroyVoice();
    }
    if (m_soundEffectMasteringVoice != nullptr)
    {
        m_soundEffectMasteringVoice->DestroyVoice();
    }

    m_musicSourceVoice = nullptr;
    m_musicMasteringVoice = nullptr;
    m_soundEffectMasteringVoice = nullptr;
    m_soundEffectReverbVoiceSmallRoom = nullptr;
    m_musicReverbVoiceSmallRoom = nullptr;
    m_musicEngine = nullptr;
    m_soundEffectEngine = nullptr;
}

void Audio::Start()
{
    if (m_engineExperiencedCriticalError)
    {
        return;
    }

    HRESULT hr = m_musicSourceVoice->Start(0);
	SuspendMusic();
	SuspendSFX();

	if SUCCEEDED(hr) {
        m_isAudioStarted = true;
    }
    else
    {
        m_engineExperiencedCriticalError = true;
    }
}

// This sample processes audio buffers during the render cycle of the application.
// As long as the sample maintains a high-enough frame rate, this approach should
// not glitch audio. In game code, it is best for audio buffers to be processed
// on a separate thread that is not synced to the main render loop of the game.
void Audio::Render()
{
    if (m_engineExperiencedCriticalError)
    {
        m_engineExperiencedCriticalError = false;
        auto lastUrl = m_musicStreamer.GetUrl();
		try
		{
			ReleaseResources();
			Initialize();
			CreateResources();
		}
		catch (...)
		{
			m_engineExperiencedCriticalError = false;
			return;
		}

		if (!lastUrl.empty())
		{
			try
			{
				ReInitializeStreamer(lastUrl.c_str());	
				Start();
			}
			catch (...)
			{
				m_engineExperiencedCriticalError = true;
				return;
			}
		}
    }

    try
    {
        bool streamComplete;
        XAUDIO2_VOICE_STATE state;
        uint32 bufferLength;
        XAUDIO2_BUFFER buf = {0};

        // Use MediaStreamer to stream the buffers.
        m_musicSourceVoice->GetState(&state);
        while (state.BuffersQueued <= MAX_BUFFER_COUNT - 1)
        {
            streamComplete = m_musicStreamer.GetNextBuffer(
                m_audioBuffers[m_currentBuffer],
                STREAMING_BUFFER_SIZE,
                &bufferLength
                );

            if (bufferLength > 0)
            {
                buf.AudioBytes = bufferLength;
                buf.pAudioData = m_audioBuffers[m_currentBuffer];
                buf.Flags = (streamComplete) ? XAUDIO2_END_OF_STREAM : 0;
                buf.pContext = 0;
                ThrowIfFailed(
                    m_musicSourceVoice->SubmitSourceBuffer(&buf)
                    );

                m_currentBuffer++;
                m_currentBuffer %= MAX_BUFFER_COUNT;
            }

            if (streamComplete)
            {
                // Loop the stream.
                m_musicStreamer.Restart();
                break;
            }

            m_musicSourceVoice->GetState(&state);
        }
    }
    catch (...)
    {
        m_engineExperiencedCriticalError = true;
    }
}

void Audio::PlaySoundEffect(SoundEvent sound)
{
    XAUDIO2_BUFFER buf = {0};
    XAUDIO2_VOICE_STATE state = {0};

    if (m_engineExperiencedCriticalError) 
	{
        return;
    }

    SoundEffectData* soundEffect = &m_soundEffects[sound];
    HRESULT hr = soundEffect->m_soundEffectSourceVoice->Start();
    if FAILED(hr)
    {
        m_engineExperiencedCriticalError = true;
        return;
    }

    soundEffect->m_soundEffectSourceVoice->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);

    soundEffect->m_soundEffectSourceVoice->Stop();
    soundEffect->m_soundEffectSourceVoice->FlushSourceBuffers();
    soundEffect->m_soundEffectSourceVoice->SubmitSourceBuffer(&soundEffect->m_audioBuffer);
    soundEffect->m_soundEffectSourceVoice->Start();
	
    m_soundEffects[sound].m_soundEffectStarted = true;
}

void Audio::StopSoundEffect(SoundEvent sound)
{
    if (m_engineExperiencedCriticalError) {
        return;
    }

    HRESULT hr = m_soundEffects[sound].m_soundEffectSourceVoice->Stop();
    if FAILED(hr)
    {
        // If there's an error, then we'll recreate the engine on the next render pass
        m_engineExperiencedCriticalError = true;
        return;
    }

    m_soundEffects[sound].m_soundEffectStarted = false;
}

bool Audio::IsSoundEffectStarted(SoundEvent sound)
{
    return m_soundEffects[sound].m_soundEffectStarted;
}

void Audio::SetSoundEffectVolume(SoundEvent sound, float volume)
{
    if (m_soundEffects[sound].m_soundEffectSourceVoice != nullptr) {
        m_soundEffects[sound].m_soundEffectSourceVoice->SetVolume(volume);
    }
}

void Audio::SetAllSoundEffectVolume(float volume)
{
	for (int i = 0; i< LastSoundEvent; i++)
	{
		if (m_soundEffects[i].m_soundEffectSourceVoice != nullptr)
		{
			m_soundEffects[i].m_soundEffectSourceVoice->SetVolume(volume);
		}
	}
}

void Audio::SetSoundEffectPitch(SoundEvent sound, float pitch)
{
    if (m_soundEffects[sound].m_soundEffectSourceVoice != nullptr) {
        m_soundEffects[sound].m_soundEffectSourceVoice->SetFrequencyRatio(pitch);
    }
}

void Audio::SetSoundEffectFilter(SoundEvent sound, float frequency, float oneOverQ)
{
    if (m_soundEffects[sound].m_soundEffectSourceVoice != nullptr) {
        if (oneOverQ <= 0.1f)
        {
            oneOverQ = 0.1f;
        }
        if (oneOverQ > XAUDIO2_MAX_FILTER_ONEOVERQ)
        {
            oneOverQ = XAUDIO2_MAX_FILTER_ONEOVERQ;
        }
        XAUDIO2_FILTER_PARAMETERS params = {LowPassFilter, XAudio2CutoffFrequencyToRadians(frequency, m_soundEffects[sound].m_soundEffectSampleRate), oneOverQ};

        m_soundEffects[sound].m_soundEffectSourceVoice->SetFilterParameters(&params);
    }
}

// Restarts the audio streams. A call to this method must match a previous call
// to SuspendAudio. This method causes audio to continue where it left off.
// If there is a problem with the restart, the m_engineExperiencedCriticalError
// flag is set. The next call to Render will recreate all the resources and
// reset the audio pipeline.
void Audio::StartMusic()
{
	if (m_engineExperiencedCriticalError)
    {
        return;
    }
	if (m_musicEngine != nullptr)
	{
		HRESULT hr = m_musicEngine->StartEngine();

		if (FAILED(hr))
		{
			m_engineExperiencedCriticalError = true;
		}
		else
		{
			m_isMusicEngineStarted = true;
		}
	}
}

void Audio::StartSFX()
{
	if (m_engineExperiencedCriticalError)
    {
        return;
    }

	if (m_soundEffectEngine != nullptr)
	{
		HRESULT hr = m_soundEffectEngine->StartEngine();

		if (FAILED(hr))
		{
			m_engineExperiencedCriticalError = true;
		}
		else
		{
			m_isSFXEngineStarted = true;
		}
	}
}


// Uses the IXAudio2::StopEngine method to stop all audio immediately.
// It leaves the audio graph untouched, which preserves all effect parameters
// and effect histories (like reverb effects) voice states, pending buffers,
// cursor positions and so on.
// When the engines are restarted, the resulting audio will sound as if it had
// never been stopped except for the period of silence.
void Audio::SuspendMusic()
{
	if (m_musicEngine != nullptr)
	{
		m_musicEngine->StopEngine();
		m_isMusicEngineStarted = false;
	}
}

void Audio::SuspendSFX()
{
	if (m_soundEffectEngine != nullptr)
	{
		m_soundEffectEngine->StopEngine();
		m_isSFXEngineStarted = false;
	}
}

bool Audio::IsMusicStarted()
{
	return m_isMusicEngineStarted;
}

bool Audio::IsSFXStarted()
{
	return m_isSFXEngineStarted;
}

void Audio::KillEngine()
{
	SuspendMusic();
	SuspendSFX();
	ReleaseResources();
}

void Audio::SetMusicVolume(float volume)
{
	if (m_musicSourceVoice != nullptr)
	{
		m_musicSourceVoice->SetVolume(volume);
	}
}