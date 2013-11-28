
#pragma once

using namespace DXCore;
using namespace  DXCore::BasicSprites;


class Sprite
{

protected:

	float   rot;
	float2  pos;
	float2  scale;


public:

	Microsoft::WRL::ComPtr<ID3D11Texture2D>		 _texture;
	Windows::Foundation::Rect				 _windowRect;
	Windows::Foundation::Rect				 textureSize;


	virtual void	SetScale(float2 _scale);
	virtual float2  GetScale();
	virtual void    SetPos(float2 _pos);
	virtual float2  GetPos();
	virtual void    SetRot(float  _rot);
	virtual float   GetRot();

	virtual void SetTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D>  texture);
	virtual void SetWindowSize(Windows::Foundation::Rect windowRect);

	virtual void Update(float timeDelta);
	virtual void Draw(BasicSprites::SpriteBatch^ m_spriteBatch);
	virtual bool IsOutOfVisibleArea();
	virtual bool IsPastPlayer();
};