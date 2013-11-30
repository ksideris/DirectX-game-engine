#include "pch.h"
#include "ParentControlledBackground.h"


using namespace BasicSprites;


ParentControlledBackground::ParentControlledBackground(){
	parent = NULL;
}


void ParentControlledBackground::SetParent(GameObject * _parent)
{
	parent = _parent;

}
void ParentControlledBackground::Update(float timeDelta)
{

}


void ParentControlledBackground::Draw(BasicSprites::SpriteBatch^ m_spriteBatch)
{
	if (parent != NULL)
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
			float2(parent->GetPos().x + textureSize.Width / 2.0 + parent->textureSize.Width*parent->GetScale().x, parent->GetPos().y),
			PositionUnits::DIPs,
			scale,
			SizeUnits::Normalized
			);
		m_spriteBatch->Draw(
			_texture_b.Get(),
			float2(parent->GetPos().x + textureSize.Width / 2.0 + parent->textureSize.Width*parent->GetScale().x, parent->GetPos().y),
			PositionUnits::DIPs,
			scale,
			SizeUnits::Normalized,
			float4(0.f, 0.f, 0.f, 1.f)
			);

	}

}