#include "pch.h"
#include "SlidingBackgroundSprite.h"


using namespace BasicSprites;


SlidingBackgroundSprite::SlidingBackgroundSprite(){
	rendermode = BackgroundRenderMode::STRETCH_HEIGHT;


	vel.x = -10.f;
}
SlidingBackgroundSprite::SlidingBackgroundSprite(BackgroundRenderMode _mode){
	rendermode = _mode;

	vel.x = -10.f;
}
void SlidingBackgroundSprite::SetTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D>  texture)
{
	_texture = texture;
	_texture_b = texture;

	D3D11_TEXTURE2D_DESC* desc = new D3D11_TEXTURE2D_DESC();

	_texture->GetDesc(desc);

	textureSize.Width = (float) desc->Width;
	textureSize.Height = (float) desc->Height;

	delete desc;


}
void SlidingBackgroundSprite::SetTextures(Microsoft::WRL::ComPtr<ID3D11Texture2D>  textureA, Microsoft::WRL::ComPtr<ID3D11Texture2D>  textureB)
{
	_texture = textureA;
	_texture_b = textureB;

	D3D11_TEXTURE2D_DESC* desc = new D3D11_TEXTURE2D_DESC();

	_texture->GetDesc(desc);

	textureSize.Width = (float) desc->Width;
	textureSize.Height = (float) desc->Height;

	delete desc;

}
void SlidingBackgroundSprite::InitSliding()
{

	scale = float2(1.f, 1.f);
	switch (rendermode)
	{
	case BackgroundRenderMode::STRETCH_HEIGHT:
		scale.y = _windowRect.Height / textureSize.Height;
		scale.x = _windowRect.Height / textureSize.Height;
		break;
	case BackgroundRenderMode::STRETCH_WIDTH:
		scale.x = _windowRect.Width / textureSize.Width;
		scale.y = _windowRect.Width / textureSize.Width;
		break;
	case BackgroundRenderMode::STRETCH:
		scale.y = _windowRect.Height / textureSize.Height;
		scale.x = _windowRect.Width / textureSize.Width;
		break;

	case BackgroundRenderMode::ORIGINAL:
	default:
		scale = float2(1.f, 1.f);
	}
	pos.x = textureSize.Width*scale.x / 2.f;

	second_pos.x = textureSize.Width*scale.x  * 1.5f;

}

void	SlidingBackgroundSprite::SetVel(float2 _vel){
	vel = _vel;
	pos.x = textureSize.Width*scale.x / 2.f;

	second_pos.x = textureSize.Width*scale.x  * 1.5f;
}
float2  SlidingBackgroundSprite::GetVel(){
	return vel;
}
void SlidingBackgroundSprite::Update(float timeDelta)
{
	if (		pos.x <= -textureSize.Width*scale.x / 2.f)
		pos.x = second_pos.x + (textureSize.Width*scale.x);
	if (second_pos.x <= -textureSize.Width*scale.x / 2.f)
		second_pos.x = pos.x + (textureSize.Width*scale.x);

	pos.x += vel.x;
	second_pos.x += vel.x;
}


void SlidingBackgroundSprite::Draw(BasicSprites::SpriteBatch^ m_spriteBatch)
{

	m_spriteBatch->Draw(
		_texture.Get(),
		float2(pos.x / _windowRect.Width, 0.5f),
		PositionUnits::Normalized,
		scale, 
		SizeUnits::Normalized
		);
	m_spriteBatch->Draw(
		_texture_b.Get(),
		float2(second_pos.x / _windowRect.Width, 0.5f),
		PositionUnits::Normalized,
		scale,
		SizeUnits::Normalized
		);
}