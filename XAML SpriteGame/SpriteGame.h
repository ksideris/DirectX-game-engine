#pragma once

#include <wrl.h>
#include "DXCore\DirectXBase.h" 
#include "pch.h" 



#include "GameEngine\Player.h"
#include "GameEngine\Asteroid.h" 
#include "GameEngine\SlidingBackgroundSprite.h"
#include "GameEngine\RocketFire.h"
#include "GameEngine\FireBall.h"
#include "GameEngine\Direct2DlightEffect.h"
 

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


	property SpriteBatch^									CurrentSpriteBatch;
	float RandFloat(float min, float max);


	BasicSprites::SpriteBatch^ m_spriteBatch;


	Player* spaceship;

	SlidingBackgroundSprite * background;
	 
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_background;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_asteroid;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_particle;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_player;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_player_spot;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_debug_point;


	std::vector<Asteroid> m_asteroidData;
	std::vector<FireBall> m_particleData;

	RocketFire * rocketFuel;


	int m_numParticlesToDraw; 


	bool				IsWithinScreenBoundaries(float2);
	void				LoadScreen();
	void				CheckScreenType(); 


	d2dLightEffect * spaceShipLight;

};