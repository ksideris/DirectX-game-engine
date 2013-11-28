
#pragma once
#include "Sprite.h"

#include "Background.h" 
#include "GameObjects/GamePlayElement.h" 
#include "Backgrounds/SlidingBackgroundSprite.h" 
#include "ParticleEffects/RocketFire.h"
#include "ParticleEffects/FireBall.h"
#include "Direct2DlightEffect.h"
#include "GameObjects/Characters/Enemy.h" 
#include "GameObjects/Characters/HorizontalSliderPlayer.h" 

using namespace DXCore;
using namespace  DXCore::BasicSprites;

class Level
{

public:
	SlidingBackgroundSprite * background;
	SlidingBackgroundSprite * foreground;
 

	float finish_pos;

	std::vector <GameObject*> all_gameobjects;
	std::vector <Background*> all_backgrounds;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_background;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_foreground;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_foreground2;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_asteroid;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_ring;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_fline;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_projectile;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_health_text;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_weapons_text;
		 
	std::vector <std::pair< std::pair<int, int>, float>> Enemies;  
	std::vector <std::pair<int, float>> AsteroidFields;
	std::vector <std::pair<int, float>> Rings;

	std::vector <float> Powerups;
	std::vector <float> Health_upg;

	std::vector < Microsoft::WRL::ComPtr<ID3D11Texture2D>   > EnemyTextures; 


	void SetWindowDependentProperties(Windows::Foundation::Rect  m_windowBounds);
	void Load(std::string level_loc, BasicSprites::SpriteBatch^ m_spriteBatch, BasicLoader^ loader);
	void Update(float timeTotal, float timeDelta, Windows::Foundation::Rect  m_windowBounds);
	~Level();
};