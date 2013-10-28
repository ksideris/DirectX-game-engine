
#pragma once

#include "DirectXBase.h"
#include "BasicSprites.h"

class Sprite
{

public:

	Windows::Foundation::Rect textureSize;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> _texture;
	Windows::Foundation::Rect _windowRect;

	float2 pos;
	float scale;

	void Update(float timeDelta);
	void SetTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture);
	void SetWindowSize(Windows::Foundation::Rect windowRect);
	void Draw(BasicSprites::SpriteBatch^ m_spriteBatch);
	bool IsOutOfVisibleArea();
};