#pragma once

#include <wrl.h>
#include "DXCore\DirectXBase.h" 
#include "pch.h" 


#include "Audio\AudioManager.h"

#include "GameEngine\GameObject.h"
#include "GameEngine\Background.h"  
#include "GameEngine\Direct2DlightEffect.h"
#include "GameEngine\Level.h"

#include "GameEngine\ParticleEffects\RocketFire.h"
#include "GameEngine\ParticleEffects\FireBall.h"
#include "GameEngine\ParticleEffects\Explosion.h"


 
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
	int score;


	HorizontalSliderPlayer* spaceship;
	RocketFire * rocketFuel;
	d2dLightEffect * spaceShipLight;

	 
	std::vector<FireBall> m_particleData;
	std::vector<Explosion> m_explosionData;

	vector < pair<GameObject*, GameObject*>> colliding;

	int m_numParticlesToDraw; 


	void DetectCollisions();
	void HandleCollisions();
	void RemoveDead();
	void LoadScreen(); 
	 

};