#pragma once

// Helper utilities to make DirectX APIs work with exceptions

namespace DXCore
{
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			// Set a breakpoint on this line to catch DirectX API errors
			throw Platform::Exception::CreateException(hr);
		}
	}
}
