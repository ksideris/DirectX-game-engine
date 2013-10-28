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
	Projectile data;
	data.pos = spaceship->pos;
	data.vel = float2(1000.0f, 0.0f);
	data.size = 10;
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


	background1 = new BackgroundSprite();
	background2 = new BackgroundSprite();

	background1->pos.x = m_windowBounds.Width / 2;
	background1->SetTexture(m_background);
	background1->SetWindowSize(m_windowBounds);

	background2->pos = background1->pos.x * 3;
	background2->SetTexture(m_background);
	background2->SetWindowSize(m_windowBounds);


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
		data.rot = RandFloat(-PI_F, PI_F);
		data.scale = RandFloat(0.1f, 1.0f);
		data.rotVel = RandFloat(-PI_F, PI_F) / (7.0f + 3.0f * data.scale);
		data.size = (180, 110);
		data.SetTexture(m_asteroid);
		data.SetWindowSize(m_windowBounds);
		m_asteroidData.push_back(data);
	}



	spaceship = new Player();

	spaceship->pos.x =   m_windowBounds.Width/2.0;
	spaceship->pos.y =  m_windowBounds.Height / 2.0;
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
	// Update the performance throttler.

	if (background1->pos.x < -m_windowBounds.Width / 2)
		background1->pos.x = background2->pos.x + (m_windowBounds.Width);
	if (background2->pos.x < -m_windowBounds.Width / 2)
		background2->pos.x = background1->pos.x + (m_windowBounds.Width);

	background1->Update(timeDelta);
	background2->Update(timeDelta);



	for (auto asteroid = m_asteroidData.begin(); asteroid != m_asteroidData.end(); asteroid++)
		asteroid->Update(timeDelta);

	for (auto particle = m_particleData.begin(); particle != m_particleData.end(); particle++)
		particle->Update(timeDelta);

	spaceship->Update(timeDelta);
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

	// Draw the background.


	background1->Draw(m_spriteBatch);
	background2->Draw(m_spriteBatch);


	// Draw the non-interactive asteroids.

	for (auto asteroid = m_asteroidData.begin(); asteroid != m_asteroidData.end(); asteroid++)
	{
		asteroid->Draw(m_spriteBatch);
		
	}

	//// Draw the interactive particles.
	if (m_particleData.size() > 0)
	{
		for (auto particle = m_particleData.begin(); particle != m_particleData.end(); particle++)
		{
			for (int i = 0; i < 10; i++)
			{
				float alpha = 1.0f;
				m_spriteBatch->Draw(
					m_particle.Get(),
					particle->pos,
					PositionUnits::DIPs,
					float2(32.0f, 32.0f),
					SizeUnits::DIPs,
					float4(0.1f, 0.02f, 0.0f, alpha),
					0.0f,
					BlendMode::Additive
					);
			}
		}
	}
	spaceship->Draw(m_spriteBatch);

	m_spriteBatch->End();

	// Render the Sample Overlay.

	m_Overlay->Render();
}

float SpriteGame::RandFloat(float min, float max)
{
	return (static_cast<float>(rand() % RAND_MAX) / static_cast<float>(RAND_MAX)) * (max - min) + min;
}

