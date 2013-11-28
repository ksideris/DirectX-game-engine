#include "pch.h"
#include "FlashingBackground.h"


using namespace BasicSprites;


FlashingBackground::FlashingBackground(){
	color = float4(1.f, 1.f, 1.f, 0.f);
}


void FlashingBackground::Update(float timeDelta)
{
	color.a += .01f;
	if (color.a > .5f)
		color.a = 0.f;
}


void FlashingBackground::Draw(BasicSprites::SpriteBatch^ m_spriteBatch)
{
	rendermode = BackgroundRenderMode::STRETCH;
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
	m_spriteBatch->Draw(
		_texture.Get(),
		float2(.5f, 0.5f),
		PositionUnits::Normalized,
		scale,
		SizeUnits::Normalized,
		color
		);
	
}