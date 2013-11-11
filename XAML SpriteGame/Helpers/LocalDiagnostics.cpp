#include "pch.h"
#include "LocalDiagnostics.h"
#include <sstream>

using namespace  Helpers;

void LocalDiagnostics::Print(float data)
{
	std::wstringstream wss;
	wss << data;
	std::wstring sdata = wss.str().c_str();
	sdata += L"\n";

	OutputDebugStringW(sdata.c_str());
}

