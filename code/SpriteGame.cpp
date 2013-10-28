//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#include "pch.h"
#include "SpriteGame.h"
#include "BasicLoader.h"

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;

using namespace BasicSprites;

SpriteGame::SpriteGame() :
m_numParticlesToDraw(SampleSettings::Performance::InitialParticleCount)
{
}

void SpriteGame::CreateDeviceIndependentResources()
{
	DirectXBase::CreateDeviceIndependentResources();

}

void SpriteGame::CreateProjectile()
{
	ParticleSystem data;
	data.pos = spaceship->pos;
	data.vel = float2(1000.0f, 0.0f);
	data.SetTexture(m_particle);
	data.SetWindowSize(m_windowBounds);
	m_particleData.push_back(data);
}

void SpriteGame::CreateDeviceResources()
{
	DirectXBase::CreateDeviceResources();

	// Create the sprite batch.

	m_spriteBatch = ref new SpriteBatch();
	unsigned int capacity = SampleSettings::Performance::ParticleCountMax + SampleSettings::NumAsteroids + 1;
	if (m_featureLevel < D3D_FEATURE_LEVEL_9_3)
	{
		capacity = min(Parameters::MaximumCapacityCompatible, capacity);
	}
	m_spriteBatch->Initialize(
		m_d3dDevice.Get(),
		capacity
		);




	// Load the sprite textures.

	BasicLoader^ loader = ref new BasicLoader(m_d3dDevice.Get(), m_wicFactory.Get());

	loader->LoadTexture(
		"Assets\\GameObjects\\m31.png",
		&m_background,
		nullptr
		);

	m_spriteBatch->AddTexture(m_background.Get());

	loader->LoadTexture(
		"Assets\\GameObjects\\ida.dds",
		&m_asteroid,
		nullptr
		);
	m_spriteBatch->AddTexture(m_asteroid.Get());

	loader->LoadTexture(
		"Assets\\GameObjects\\SpaceShip.png",
		&m_player,
		nullptr
		);
	m_spriteBatch->AddTexture(m_player.Get());



	loader->LoadTexture(
		"Assets\\GameObjects\\particle.dds",
		&m_particle,
		nullptr
		);
	m_spriteBatch->AddTexture(m_particle.Get());

	// Create the Sample Overlay.

	m_Overlay = ref new Overlay();

	m_Overlay->Initialize(
		m_d2dDevice.Get(),
		m_d2dContext.Get(),
		m_wicFactory.Get(),
		m_dwriteFactory.Get(),
		"What is the use of this?"
		);
}

void SpriteGame::CreateWindowSizeDependentResources()
{
	DirectXBase::CreateWindowSizeDependentResources();


	background = new SlidingBackgroundSprite();

	background->SetTexture(m_background);
	background->SetWindowSize(m_windowBounds);
	background->InitSliding();

	// Randomly generate some non-interactive asteroids to fit the screen.
	m_asteroidData.clear();
	for (int i = 0; i < SampleSettings::NumAsteroids; i++)
	{
		Asteroid data;
		data.pos.x = RandFloat(0.0f, m_windowBounds.Width);
		data.pos.y = RandFloat(0.0f, m_windowBounds.Height);
		float tempRot = RandFloat(-PI_F, PI_F);
		float tempMag = RandFloat(0.0f, 17.0f);
		data.vel.x = tempMag * cosf(tempRot);
		data.vel.y = tempMag * sinf(tempRot);
		data.rot = 0;//RandFloat(-PI_F, PI_F);
		data.scale =  RandFloat(0.1f, 1.0f);
		data.rotVel = RandFloat(-PI_F, PI_F) / (7.0f + 3.0f * data.scale);
		data.size = (180, 110);
		data.SetTexture(m_asteroid);
		data.SetWindowSize(m_windowBounds);
		m_asteroidData.push_back(data);
	}



	spaceship = new Player();

	spaceship->pos.x = (float) m_windowBounds.Width / 2.0;
	spaceship->pos.y = (float) m_windowBounds.Height / 2.0;
	float tempRot = RandFloat(-PI_F, PI_F);
	float tempMag = RandFloat(0.0f, 17.0f);
	spaceship->vel.x = 10;
	spaceship->vel.y = 0;
	spaceship->rot = (0, 0);
	spaceship->scale = .25;
	spaceship->SetTexture(m_player);
	spaceship->SetWindowSize(m_windowBounds);

	m_Overlay->UpdateForWindowSizeChange();
}

void SpriteGame::Update(float timeTotal, float timeDelta)
{
	background->Update(timeDelta);
	spaceship->Update(timeDelta);

	for (auto asteroid = m_asteroidData.begin(); asteroid != m_asteroidData.end(); asteroid++)
	{
		asteroid->Update(timeDelta);
		if (spaceship->IsColliding(*asteroid))
		{
			asteroid = m_asteroidData.erase(asteroid);

			if (asteroid == m_asteroidData.end())
				break;
		}
	}
	for (auto particle = m_particleData.begin(); particle != m_particleData.end(); particle++)
	{
		particle->Update(timeDelta);
		 
		if (particle->IsOutOfVisibleArea())
		{
			particle = m_particleData.erase(particle);

			if (particle == m_particleData.end())
				break;
		}
	} 


}

void SpriteGame::Render()
{
	m_d3dContext->OMSetRenderTargets(
		1,
		m_d3dRenderTargetView.GetAddressOf(),
		nullptr
		);

	m_d3dContext->ClearRenderTargetView(
		m_d3dRenderTargetView.Get(),
		reinterpret_cast<float*>(&D2D1::ColorF(D2D1::ColorF::MidnightBlue))
		);

	m_spriteBatch->Begin();
	 

	background->Draw(m_spriteBatch);

	for (auto asteroid = m_asteroidData.begin(); asteroid != m_asteroidData.end(); asteroid++)
		asteroid->Draw(m_spriteBatch); 

	for (auto particle = m_particleData.begin(); particle != m_particleData.end(); particle++)
		particle->Draw(m_spriteBatch);

	spaceship->Draw(m_spriteBatch);
	 

	m_spriteBatch->End(); 

	// Render the Sample Overlay.

	m_Overlay->Render();
}

float SpriteGame::RandFloat(float min, float max)
{
	return (static_cast<float>(rand() % RAND_MAX) / static_cast<float>(RAND_MAX)) * (max - min) + min;
}

