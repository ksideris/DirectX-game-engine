
#pragma once
#include "../Background.h"



class ParentControlledBackground : public Background
{

	GameObject *parent;
public:

	Microsoft::WRL::ComPtr<ID3D11Texture2D>		 _texture_b;
	ParentControlledBackground();
	void SetParent(GameObject * _parent);
	void Update(float timeDelta);
	void Draw(BasicSprites::SpriteBatch^ m_spriteBatch);

};