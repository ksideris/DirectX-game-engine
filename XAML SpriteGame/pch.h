#pragma once
 

#include <wrl.h>
#ifdef W8_1

#include <d3d11_2.h>
#include <d2d1_2.h>
#include <dwrite_2.h>
#else
#include <d3d11_1.h>
#include <d2d1_1.h>
#include <dwrite_1.h>
#endif
#include <d2d1effects.h>
#include <wincodec.h>
#include <ppltasks.h>
#include <agile.h>

#define XM_STRICT_VECTOR4 1
#include <DirectXMath.h>

#define XAUDIO2_HELPER_FUNCTIONS 1
#include <xaudio2.h>
#include <xaudio2fx.h>

#include <mmreg.h>
#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>

#include <XInput.h>

#include <stdio.h>
#include <strsafe.h>
#include <vector>
#include <list>
#include <memory>
#include <map>
#include <queue>
#include <string>
#include <set>
#include <sstream>

#include "DXCore\BasicLoader.h"
#include "DXCore\SpriteWorks.h"
#include "SpriteGame.h"


 
#include "App.xaml.h"