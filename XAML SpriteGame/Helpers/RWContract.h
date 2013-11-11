#pragma once

#include <ppltasks.h>


namespace Helpers
{
	ref class RWContract
	{
	private:
		Windows::Storage::StorageFolder^ m_location;
		Platform::String^ m_locationPath;

	internal:
		RWContract();
		RWContract(
			_In_ Windows::Storage::StorageFolder^ folder
			);

		Platform::Array<byte>^ ReadData(
			_In_ Platform::String^ filename
			);

		concurrency::task<Platform::Array<byte>^> ReadDataAsync(
			_In_ Platform::String^ filename
			);

		uint32 WriteData(
			_In_ Platform::String^ filename,
			_In_ const Platform::Array<byte>^ fileData
			);

		concurrency::task<void> WriteDataAsync(
			_In_ Platform::String^ filename,
			_In_ const Platform::Array<byte>^ fileData
			);
	};
}