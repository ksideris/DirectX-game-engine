//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

namespace AudioEngine
{
	class MediaStreamer
	{
	private:
		WAVEFORMATEX    m_waveFormat;
		uint32          m_maxStreamLengthInBytes;
		std::wstring 	m_url;

	public:
		Microsoft::WRL::ComPtr<IMFSourceReader> m_reader;
		Microsoft::WRL::ComPtr<IMFMediaType> m_audioType;

	public:
		MediaStreamer();
		~MediaStreamer();

		WAVEFORMATEX& GetOutputWaveFormatEx()
		{
			return m_waveFormat;
		}

		UINT32 GetMaxStreamLengthInBytes()
		{
			return m_maxStreamLengthInBytes;
		}

		const std::wstring GetUrl()
		{
			return m_url;
		}

		void Initialize(_In_ const WCHAR* url);
		bool GetNextBuffer(uint8* buffer, uint32 maxBufferSize, uint32* bufferLength);
		void ReadAll(uint8* buffer, uint32 maxBufferSize, uint32* bufferLength);
		void Restart();
	};
}