#include "pch.h"
#include "SlidingBackgroundSprite.h"


using namespace BasicSprites;

void SlidingBackgroundSprite::InitSliding()
{

	pos.x = _windowRect.Width / 2;

	second_pos.x = _windowRect.Width  * 1.5;

}

void SlidingBackgroundSprite::Update(float timeDelta)
{
	if (pos.x < -_windowRect.Width / 2)
		pos.x = second_pos.x + (_windowRect.Width);
	if (second_pos.x < -_windowRect.Width / 2)
		second_pos.x = pos.x + (_windowRect.Width);

	pos.x -= vel;
	second_pos.x -= vel;
}


void SlidingBackgroundSprite::Draw(BasicSprites::SpriteBatch^ m_spriteBatch)
{
	m_spriteBatch->Draw(
		_texture.Get(),
		float2(pos.x / _windowRect.Width, 0.5f),
		PositionUnits::Normalized,
		float2(_windowRect.Width / textureSize.Width, _windowRect.Height / textureSize.Height),
		SizeUnits::Normalized
		);
	m_spriteBatch->Draw(
		_texture.Get(),
		float2(second_pos.x / _windowRect.Width, 0.5f),
		PositionUnits::Normalized,
		float2(_windowRect.Width / textureSize.Width, _windowRect.Height / textureSize.Height),
		SizeUnits::Normalized
		);
}