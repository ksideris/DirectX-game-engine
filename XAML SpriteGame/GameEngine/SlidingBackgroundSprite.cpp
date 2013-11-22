#include "pch.h"
#include "SlidingBackgroundSprite.h"


using namespace BasicSprites;

void SlidingBackgroundSprite::InitSliding()
{
	vel.x = -10.f;
	pos.x = textureSize.Width / 2.f;

	second_pos.x = textureSize.Width  * 1.5f;

}

void	SlidingBackgroundSprite::SetVel(float2 _vel){
	vel = _vel;
}
float2  SlidingBackgroundSprite::GetVel(){
	return vel;
}
void SlidingBackgroundSprite::Update(float timeDelta)
{
	if (pos.x <= -textureSize.Width / 2.f)
		pos.x = second_pos.x + (textureSize.Width);
	if (second_pos.x <= -textureSize.Width / 2.f)
		second_pos.x = pos.x + (textureSize.Width);

	pos.x += vel.x;
	second_pos.x += vel.x;
}


void SlidingBackgroundSprite::Draw(BasicSprites::SpriteBatch^ m_spriteBatch)
{
	m_spriteBatch->Draw(
		_texture.Get(),
		float2(pos.x / _windowRect.Width, 0.5f),
		PositionUnits::Normalized,
		float2(1.0f, 1.0f),//_windowRect.Width / textureSize.Width, _windowRect.Height / textureSize.Height),
		SizeUnits::Normalized
		);
	m_spriteBatch->Draw(
		_texture.Get(),
		float2(second_pos.x / _windowRect.Width, 0.5f),
		PositionUnits::Normalized,
		float2(1.0f,1.0f),
		SizeUnits::Normalized
		);
}