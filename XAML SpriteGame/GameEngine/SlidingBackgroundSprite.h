
#pragma once
#include "Sprite.h"

enum BackgroundRenderMode{
	ORIGINAL = 1,
	STRETCH_HEIGHT = 2,
	STRETCH_WIDTH = 3,
	STRETCH = 4
};

class SlidingBackgroundSprite : public Sprite
{
	float2 vel;
	float2 second_pos;
	float2 tscale;
	BackgroundRenderMode rendermode;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>		 _texture_b;
public:
	SlidingBackgroundSprite();
	SlidingBackgroundSprite(BackgroundRenderMode _mode);
	void InitSliding();
	void Update(float timeDelta);
	void Draw(BasicSprites::SpriteBatch^ m_spriteBatch);
	virtual void SetTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D>  texture);
	virtual void SetTextures(Microsoft::WRL::ComPtr<ID3D11Texture2D>  textureA, Microsoft::WRL::ComPtr<ID3D11Texture2D>  textureB);
	virtual void    SetVel(float2 _vel);  //Convert To properties!
	virtual float2  GetVel();
};