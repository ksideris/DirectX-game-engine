#pragma once

#include <wrl.h>
#include "DXCore\DirectXBase.h" 
#include "pch.h" 

#include "Audio\AudioManager.h"

#include "GameEngine\GameObject.h"
#include "GameEngine\GlobalData.h"

 
enum GameState
{
	Menu = 1,
	Playing = 2,
	GameOver = 3,
	Paused = 4,
	LevelComplete = 5,
	LevelWon = 6,
	LevelLost = 7
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

internal:  

	GameState gamestate;
	float last_explosion;
	property SpriteBatch^ CurrentSpriteBatch;
	BasicSprites::SpriteBatch^ m_spriteBatch;
	
	vector < pair<GameObject*, GameObject*>> colliding;
	int m_numParticlesToDraw; 

	void DetectCollisions();
	void HandleCollisions();
	void RemoveDead();
	void LoadScreen(); 
	 

};