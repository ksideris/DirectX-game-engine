#pragma once

#include <wrl.h>
#include "DXCore\DirectXBase.h" 
#include "pch.h" 


#include "Audio\AudioManager.h"

#include "GameEngine\HorizontalSliderPlayer.h"
#include "GameEngine\Asteroid.h" 
#include "GameEngine\SlidingBackgroundSprite.h"
#include "GameEngine\RocketFire.h"
#include "GameEngine\FireBall.h"
#include "GameEngine\Direct2DlightEffect.h"
#include "GameEngine\Enemy.h"
#include "GameEngine\Level.h"
 

ref class SpriteGame sealed : public DirectXBase
{

public:
	SpriteGame();
	virtual void CreateDeviceIndependentResources() override;
	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void Render() override;
	 

	void Update(float timeTotal, float timeDelta);


	void CreateProjectile();

internal: 


	Level* level;

	void CreateEnemyProjectile(Enemy* enemy);

	property SpriteBatch^									CurrentSpriteBatch;
	  
	BasicSprites::SpriteBatch^ m_spriteBatch;
	 
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_background;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_asteroid;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_particle;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_player;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_player_spot;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_debug_point;



	HorizontalSliderPlayer* spaceship;
	RocketFire * rocketFuel;
	d2dLightEffect * spaceShipLight;


	 
	std::vector<Asteroid> m_asteroidData;
	std::vector<Asteroid> m_asteroidFragments;
	std::vector<FireBall> m_particleData;


	int m_numParticlesToDraw; 

	 
	void				LoadScreen();
	void				CheckScreenType(); 

	 

};