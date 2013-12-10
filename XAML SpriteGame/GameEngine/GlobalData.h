#pragma once

#include "GameEngine\GameObject.h"
#include "GameEngine\Background.h"  
#include "GameEngine\Backgrounds\FlashingBackground.h"  
#include "GameEngine\Backgrounds\ParentControlledBackground.h"  

#include "GameEngine\Level.h"
#include "GameEngine\ParticleEffects\RocketFire.h"
#include "GameEngine\ParticleEffects\FireBall.h"
#include "GameEngine\ParticleEffects\Explosion.h"


class GlobalData
{
public:
	Level* level;

	float last_explosion; 

	BasicSprites::SpriteBatch^ m_spriteBatch;

	Windows::Foundation::Rect        m_windowBounds;

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
	FlashingBackground * bad_health_background; 

	std::vector<Explosion> m_explosionData;
	std::vector<FireBall> bullets;
	std::vector<FireBall> enemybullets;

	vector < pair<GameObject*, GameObject*>> colliding;

	int m_numParticlesToDraw;
};

class GlobalHelper
{
	static GlobalData* theData;
public:
	static GlobalData* getData();


};