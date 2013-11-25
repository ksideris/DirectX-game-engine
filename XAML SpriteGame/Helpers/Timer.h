//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once


namespace Helpers
{
	ref class Timer
	{
	private:
		LARGE_INTEGER m_frequency;
		LARGE_INTEGER m_currentTime;
		LARGE_INTEGER m_startTime;
		LARGE_INTEGER m_lastTime;
		float m_total;
		float m_delta;

	internal:
		Timer();
		void Reset();
		void Reset(LONGLONG offset);
		void Update();
		property float Total
		{
			float get();
		}
		property float Delta
		{
			float get();
		}
		property LONGLONG CurrentTime
		{
			LONGLONG get();
		}
	};
}