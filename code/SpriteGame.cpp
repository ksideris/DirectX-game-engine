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

	// Create the performance throttler.



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
		&m_background1,
		nullptr
		);


	/*D3D11_TEXTURE2D_DESC* desc =new D3D11_TEXTURE2D_DESC() ;
		
	m_background1->GetDesc(desc);
	desc->Height;*/


	m_spriteBatch->AddTexture(m_background1.Get());
	loader->LoadTexture(
		"Assets\\GameObjects\\m31.png",
		&m_background2,
		nullptr
		);
	m_spriteBatch->AddTexture(m_background2.Get());


	loader->LoadTexture(
		"Assets\\GameObjects\\ida.dds",
		&m_asteroid,
		nullptr
		);
	m_spriteBatch->AddTexture(m_asteroid.Get());

	/*************SPACESHIP CODE*********************/
	spaceship = new SpaceShip();



	spaceship->pos.x = RandFloat(0.0f, m_windowBounds.Width);
	spaceship->pos.y = RandFloat(0.0f, m_windowBounds.Height);
	float tempRot = RandFloat(-PI_F, PI_F);
	float tempMag = RandFloat(0.0f, 17.0f);
	spaceship->vel.x = 0;
	spaceship->vel.y = 0;
	spaceship->rot = (0, 0);
	spaceship->scale = 1;

	loader->LoadTexture(
		"Assets\\GameObjects\\SpaceShip.png",
		&spaceship->text,
		nullptr
		);
	m_spriteBatch->AddTexture(spaceship->text.Get());
	spaceship->size = (200, 200);

	/*************SPACESHIP END*********************/
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

	background1_pos = m_windowBounds.Width / 2;

	background2_pos = background1_pos * 3;
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
		m_asteroidData.push_back(data);
	}
	/*

	if (m_particleData.size() == 0)
	{
	// Initialize the interactive particle buffer to fill the window if it is empty.

	for (int i = 0; i < SampleSettings::Performance::ParticleCountMax; i++)
	{
	Projectile data;
	data.pos.x = RandFloat(0.0f, m_windowBounds.Width);
	data.pos.y = RandFloat(0.0f, m_windowBounds.Height);
	data.vel = float2(0.0f, 0.0f);
	m_particleData.push_back(data);
	}
	}
	else
	{
	// Otherwise, move the interactive particles to fit within the screen.

	for (auto particle = m_particleData.begin(); particle != m_particleData.end(); particle++)
	{
	if (particle->pos.x > m_windowBounds.Width)
	{
	particle->pos.x = m_windowBounds.Width;
	}
	if (particle->pos.y > m_windowBounds.Height)
	{
	particle->pos.y = m_windowBounds.Height;
	}
	}
	}*/

	m_Overlay->UpdateForWindowSizeChange();
}

void SpriteGame::Update(float timeTotal, float timeDelta)
{
	// Update the performance throttler.

	if (background1_pos < -m_windowBounds.Width / 2)
		background1_pos = background2_pos + (m_windowBounds.Width);
	if (background2_pos < -m_windowBounds.Width / 2)
		background2_pos = background1_pos + (m_windowBounds.Width);
	background1_pos -= 5;
	background2_pos -= 5;


	m_numParticlesToDraw = 500;
	// Update the non-interactive asteroids.
	// Their behavior is to drift across the window with a fixed translational and rotational
	// velocity.  Upon crossing a boundary outside the window, their position wraps.

	spaceship->vel = spaceship->vel + spaceship->accel*timeDelta;
	spaceship->pos = spaceship->pos + spaceship->vel * timeDelta; /**spaceship code**/
	if (spaceship->pos.x > m_windowBounds.Width - 100)
	{
		spaceship->pos.x = m_windowBounds.Width - 100;
		spaceship->accel.x = 0;
		spaceship->vel.x = 0;
	}
	if (spaceship->pos.y > m_windowBounds.Height - 100)
	{
		spaceship->pos.y = m_windowBounds.Height - 100;
		spaceship->accel.y = 0;
		spaceship->vel.y = 0;
	}
	if (spaceship->pos.x < 100)
	{
		spaceship->pos.x = 100;
		spaceship->accel.x = 0;
		spaceship->vel.x = 0;
	}
	if (spaceship->pos.y < 100)
	{
		spaceship->pos.y = 100;
		spaceship->accel.y = 0;
		spaceship->vel.y = 0;
	}
	for (auto asteroid = m_asteroidData.begin(); asteroid != m_asteroidData.end(); asteroid++)
	{
		asteroid->vel.x = asteroid->vel.x - asteroid->vel.x / 10 * timeDelta;
		if (abs(asteroid->vel.x) < .1)
			asteroid->vel = (0, 0);
	}

	if (m_particleData.size() > 0)
	{


		for (auto particle = m_particleData.begin(); particle != m_particleData.end(); particle++)
		{
			particle->pos = particle->pos + particle->vel * timeDelta;
			for (auto asteroid = m_asteroidData.begin(); asteroid != m_asteroidData.end(); asteroid++)
			{



				if (asteroid->IsColliding(particle->pos))
				{
					asteroid->vel = particle->vel;
					particle->vel = -particle->vel;
				}
			}


		}


	}
	for (auto asteroid = m_asteroidData.begin(); asteroid != m_asteroidData.end(); asteroid++)
	{


		static const float border = 100.0f;
		asteroid->pos = asteroid->pos + asteroid->vel * timeDelta;
		if (asteroid->vel.x < 0)
		{
			if (asteroid->pos.x < -border)
			{
				asteroid->pos.x = m_windowBounds.Width + border;
			}
		}
		else
		{
			if (asteroid->pos.x > m_windowBounds.Width + border)
			{
				asteroid->pos.x = -border;
			}
		}
		if (asteroid->vel.y < 0)
		{
			if (asteroid->pos.y < -border)
			{
				asteroid->pos.y = m_windowBounds.Height + border;
			}
		}
		else
		{
			if (asteroid->pos.y > m_windowBounds.Height + border)
			{
				asteroid->pos.y = -border;
			}
		}

		asteroid->rot += asteroid->rotVel * timeDelta;
		if (asteroid->rot > PI_F)
		{
			asteroid->rot -= 2.0f * PI_F;
		}
		if (asteroid->rot < -PI_F)
		{
			asteroid->rot += 2.0f * PI_F;
		}


	}
	if (m_particleData.size() > 0)
	{
		for (auto particle = m_particleData.begin(); particle != m_particleData.end(); particle++)
		{
			particle->pos = particle->pos + particle->vel * timeDelta;

		}
	}
	spaceship->accel = (0, 0);
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

	m_spriteBatch->Draw(
		m_background1.Get(),
		float2(background1_pos / m_windowBounds.Width, 0.5f),
		PositionUnits::Normalized,
		float2(1.0f, 1.0f),
		SizeUnits::Normalized
		);
	m_spriteBatch->Draw(
		m_background2.Get(),
		float2(background2_pos / m_windowBounds.Width, 0.5f),
		PositionUnits::Normalized,
		float2(1.0f, 1.0f),
		SizeUnits::Normalized
		);


	// Draw the non-interactive asteroids.

	for (auto asteroid = m_asteroidData.begin(); asteroid != m_asteroidData.end(); asteroid++)
	{
		m_spriteBatch->Draw(
			m_asteroid.Get(),
			asteroid->pos,
			PositionUnits::DIPs,
			float2(1.0f, 1.0f) * asteroid->scale,
			SizeUnits::Normalized,
			float4(0.8f, 0.8f, 1.0f, 1.0f),
			asteroid->rot
			);
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
	m_spriteBatch->Draw(
		spaceship->text.Get(),
		spaceship->pos,
		PositionUnits::DIPs,
		float2(1.0f, 1.0f) * spaceship->scale,
		SizeUnits::Normalized,
		float4(0.8f, 0.8f, 1.0f, 1.0f),
		spaceship->rot
		);

	m_spriteBatch->End();

	// Render the Sample Overlay.

	m_Overlay->Render();
}

float SpriteGame::RandFloat(float min, float max)
{
	return (static_cast<float>(rand() % RAND_MAX) / static_cast<float>(RAND_MAX)) * (max - min) + min;
}

void SpriteGame::AddRepulsor(_In_ uint32 id, _In_ float2 position)
{
	m_repulsors[id] = position;
}

void SpriteGame::MoveRepulsor(_In_ uint32 id, _In_ float2 position)
{
	m_repulsors[id] = position;
}

void SpriteGame::RemoveRepulsor(_In_ uint32 id)
{
	m_repulsors.erase(id);
}
