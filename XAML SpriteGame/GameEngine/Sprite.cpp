#include "pch.h"
#include "Sprite.h"
#include "GlobalData.h"

using namespace BasicSprites;
 
void Sprite::SetTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D>  texture)
{
	_texture = texture;
	D3D11_TEXTURE2D_DESC* desc =new D3D11_TEXTURE2D_DESC() ;

	 _texture->GetDesc(desc);

	textureSize.Width = (float)desc->Width;
	textureSize.Height = (float) desc->Height;

	delete desc;

}
 

void	Sprite::SetScale(float2 _scale){
	scale = _scale;
}
float2  Sprite::GetScale(){
	return scale;
}
void    Sprite::SetPos(float2 _pos){
	pos = _pos;
}
float2  Sprite::GetPos(){
	return pos;
}
void    Sprite::SetRot(float  _rot){
	rot = _rot;
}
float   Sprite::GetRot(){
	return rot;
}


bool Sprite::IsOutOfVisibleArea()
{
	if (pos.x > GlobalHelper::getData()->m_windowBounds.Width + textureSize.Width  * scale.x)
	{
		return true;
	}
	if (pos.y > GlobalHelper::getData()->m_windowBounds.Height + textureSize.Height   * scale.y)
	{
		return true;
	}
	if (pos.x < -textureSize.Width  * scale.x)
	{
		return true;
	}
	if (pos.y < 0)
	{
		return true;
	}

	return false;
}
bool Sprite::IsPastPlayer()
{
	 
	if (pos.x < -textureSize.Width  * scale.x)
	{
		return true;
	} 

	return false;
}
void Sprite::Update(float timeDelta)
{

}
void Sprite::Draw(BasicSprites::SpriteBatch^ m_spriteBatch)
{
	m_spriteBatch->Draw(
		_texture.Get(),
		float2(pos.x / GlobalHelper::getData()->m_windowBounds.Width, 0.5f),
		PositionUnits::Normalized,
		float2(GlobalHelper::getData()->m_windowBounds.Width / textureSize.Width, GlobalHelper::getData()->m_windowBounds.Height / textureSize.Height),
		SizeUnits::Normalized
		);
}