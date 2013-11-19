
#pragma once
#include "Sprite.h"


 

#include "HorizontalSliderPlayer.h"
#include "Asteroid.h" 
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

	std::vector<Asteroid> m_asteroidData; 
	std::vector<Asteroid> m_asteroidFragments;
	std::vector<FireBall> m_particleData;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_background;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_asteroid;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_particle;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_player;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_player_spot;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_debug_point;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_ground;
	 
	std::vector <std::pair<float, int>> Enemies;
 
	std::vector <std::pair<float, int>> AsteroidFields;

	std::vector <std::pair<float, int>> Rings; 

	Ground * ground;


	void Load(std::string level_loc, BasicSprites::SpriteBatch^ m_spriteBatch, BasicLoader^ loader);
	void Update(float timeTotal, float timeDelta, Windows::Foundation::Rect  m_windowBounds);
};