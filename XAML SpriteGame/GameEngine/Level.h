
#pragma once
#include "Sprite.h"


 

#include "HorizontalSliderPlayer.h"
#include "PassiveObject.h" 
#include "SlidingBackgroundSprite.h"
#include "RocketFire.h"
#include "FireBall.h"
#include "Direct2DlightEffect.h"
#include "Enemy.h"
#include "Ground.h"

using namespace DXCore;
using namespace  DXCore::BasicSprites;

class Level
{

public:
	SlidingBackgroundSprite * background;

	std::vector<PassiveObject> passive_objects;
	 
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_background;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_asteroid;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_ring;



	 
	std::vector <std::pair<int,float>> Enemies;
 
	std::vector <std::pair<int, float>> AsteroidFields;

	std::vector <std::pair<int, float>> Rings;

	Ground * ground;


	void Load(std::string level_loc, BasicSprites::SpriteBatch^ m_spriteBatch, BasicLoader^ loader);
	void Update(float timeTotal, float timeDelta, Windows::Foundation::Rect  m_windowBounds);
};