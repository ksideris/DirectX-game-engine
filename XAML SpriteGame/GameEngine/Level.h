
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
	SlidingBackgroundSprite * foreground;

	std::vector<PassiveObject> passive_objects;
	std::vector<Enemy> enemies;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_background;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_foreground;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_foreground2;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_asteroid;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_ring; 
	Microsoft::WRL::ComPtr<ID3D11Texture2D>		_projectile;
		 
	std::vector <std::pair< std::pair<int, int>, float>> Enemies;
 
	std::vector <std::pair<int, float>> AsteroidFields;

	std::vector < std::pair<int, float>> Rings;
	
	std::vector < Microsoft::WRL::ComPtr<ID3D11Texture2D>   > EnemyTextures;
	void SetWindowDependentProperties(Windows::Foundation::Rect  m_windowBounds);
	void Load(std::string level_loc, BasicSprites::SpriteBatch^ m_spriteBatch, BasicLoader^ loader);
	void Update(float timeTotal, float timeDelta, Windows::Foundation::Rect  m_windowBounds);
};