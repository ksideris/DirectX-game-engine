#include "pch.h"

#include "Sprite.h"

using namespace BasicSprites;
 
void Sprite::Update(float timeDelta)
{

}
void Sprite::SetTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture)
{
	_texture = texture;
	D3D11_TEXTURE2D_DESC* desc =new D3D11_TEXTURE2D_DESC() ;

	_texture->GetDesc(desc);

	textureSize.Width = desc->Width;
	textureSize.Height = desc->Height;

	delete desc;

}

void Sprite::SetWindowSize(Windows::Foundation::Rect windowRect)
{
	_windowRect = windowRect;
}


void Sprite::Draw(BasicSprites::SpriteBatch^ m_spriteBatch)
{
	m_spriteBatch->Draw(
		_texture.Get(),
		float2(pos.x/_windowRect.Width, 0.5f), 
		PositionUnits::Normalized,
		float2(_windowRect.Width / textureSize.Width, _windowRect.Height / textureSize.Height),
		SizeUnits::Normalized
		);
}