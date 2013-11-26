#pragma once

#include <wrl.h>
#include "DXCore\DirectXBase.h" 
#include "pch.h" 


#include "Audio\AudioManager.h"

#include "GameEngine\HorizontalSliderPlayer.h"
#include "GameEngine\PassiveObject.h" 
#include "GameEngine\SlidingBackgroundSprite.h"
#include "GameEngine\RocketFire.h"
#include "GameEngine\FireBall.h"
#include "GameEngine\Direct2DlightEffect.h"
#include "GameEngine\Enemy.h"
#include "GameEngine\Level.h"
#include "GameEngine\Explosion.h"


 
enum GameState
{
	Menu = 1,
	Playing = 2,
	GameOver = 3,
	Paused = 4
};

ref class SpriteGame sealed : public DirectXBase
{

public:
	SpriteGame();
	virtual void CreateDeviceIndependentResources() override;
	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void Render() override;
	virtual void LoadLevel(Platform::String^ level) ;
	 

	void Update(float timeTotal, float timeDelta);


	void CreateProjectile();

internal: 


	GameState gamestate;
	Level* level;
	 

	property SpriteBatch^									CurrentSpriteBatch;
	  
	BasicSprites::SpriteBatch^ m_spriteBatch;
	 
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_background;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_asteroid;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_particle;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_player;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_player_spot;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_debug_point;


	float time_passed;
	int score = 0;


	HorizontalSliderPlayer* spaceship;
	RocketFire * rocketFuel;
	d2dLightEffect * spaceShipLight;


	  
	std::vector<PassiveObject> m_asteroidFragments;
	std::vector<FireBall> m_particleData;
	std::vector<Explosion> m_explosionData;


	int m_numParticlesToDraw; 

	 
	void				LoadScreen(); 
	 

};