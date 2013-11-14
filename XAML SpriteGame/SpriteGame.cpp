#include "pch.h"
#include "SpriteGame.h" 
#include <DirectXMath.h>    

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace DirectX;
using namespace Windows::Graphics::Display;
using namespace DXCore;


SpriteGame::SpriteGame()
{
}

void SpriteGame::CreateDeviceIndependentResources()
{
	DirectXBase::CreateDeviceIndependentResources();

	AudioManager::Initialize();
	AudioManager::SetMainMenuMusic();

	AudioManager::IsSFXStarted = true;
	if (true)
	{
		AudioManager::AudioEngineInstance.StartSFX();
	}
	else
	{
		AudioManager::AudioEngineInstance.SuspendSFX();
	}

	AudioManager::IsMusicStarted = true;
	if (true)
	{
		AudioManager::AudioEngineInstance.StartMusic();
	}
	else
	{
		AudioManager::AudioEngineInstance.SuspendMusic();
	}

	AudioManager::SetMusicVolume(60);
	AudioManager::SetSFXVolume(60);

}

void SpriteGame::CreateProjectile()
{
	FireBall data;
	data.SetPos(spaceship->GetPos());
	data.vel = float2(1000.0f* cos(spaceship->GetRot()), -1000.0f*sin(spaceship->GetRot()));
	data.SetScale(float2(1.0f, 1.0f));
	data.SetTexture(m_particle);
	data.setCollisionGeometryForParticle(float2(20, 20), data.GetPos());
	data.SetWindowSize(m_windowBounds);
	m_particleData.push_back(data);
	AudioManager::AudioEngineInstance.PlaySoundEffect(Shoot);
}

void SpriteGame::CreateDeviceResources()
{
	DirectXBase::CreateDeviceResources();


	m_spriteBatch = ref new SpriteBatch();
	unsigned int capacity = 10000;

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
		"Assets\\GameObjects\\spotlight.png",
		&m_player_spot,
		nullptr
		);

	m_spriteBatch->AddTexture(m_player_spot.Get());

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
		"Assets\\GameObjects\\particle.png",
		&m_particle,
		nullptr
		);
	m_spriteBatch->AddTexture(m_particle.Get());

	loader->LoadTexture(
		"Assets\\GameObjects\\debug.png",
		&m_debug_point,
		nullptr
		);
	m_spriteBatch->AddTexture(m_debug_point.Get());


	spaceship = new Player();
	rocketFuel = new RocketFire();
	spaceShipLight = new d2dLightEffect();
	spaceShipLight->initWithImageFromFile(m_d2dContext, m_wicFactory);

	background = new SlidingBackgroundSprite();



}

void SpriteGame::CreateWindowSizeDependentResources()
{
	DirectXBase::CreateWindowSizeDependentResources();


	// Randomly generate some non-interactive asteroids to fit the screen.
	m_asteroidData.clear();
	for (int i = 0; i < 100; i++)
	{
		Asteroid data;
		data.SetPos(float2(RandFloat(0.0f, m_windowBounds.Width), RandFloat(0.0f, m_windowBounds.Height)));
		float tempRot = RandFloat(-PI_F, PI_F);
		float tempMag = RandFloat(0.0f, 17.0f);
		float tempScale = RandFloat(0.1f, 1.0f);

		data.SetVel(float2(tempMag * cosf(tempRot), tempMag * sinf(tempRot)));
		data.SetRot(0);
		data.SetScale(float2(tempScale, tempScale));
		data.SetRotVel(RandFloat(-PI_F, PI_F) / (7.0f + 3.0f * tempScale));
		data.SetTexture(m_asteroid);
		data.SetWindowSize(m_windowBounds);


		data.size = float2(180, 110);
		data.lifeTime = -1;
		m_asteroidData.push_back(data);
	}



	spaceship->SetPos(float2(m_windowBounds.Width / 2.0f, m_windowBounds.Height / 2.0f));
	float tempRot = RandFloat(-PI_F, PI_F);
	float tempMag = RandFloat(0.0f, 17.0f);

	spaceship->SetVel(float2(0, 0));
	spaceship->SetRot(0);
	spaceship->SetRotVel(0);
	spaceship->SetScale(float2(.5, .5));
	spaceship->SetTexture(m_player);

	spaceship->spot_texture = m_player_spot;
	spaceship->SetWindowSize(m_windowBounds);

	rocketFuel->SetPos(spaceship->GetPos());
	rocketFuel->SetTexture(m_particle);
	spaceship->AddChild(float2(spaceship->textureSize.Width / 2.0*spaceship->GetScale().x, spaceship->textureSize.Height / 2.0*spaceship->GetScale().y), rocketFuel);



	spaceship->setForwardTriangleCollisionGeometry(spaceship->GetTopLeft(), spaceship->GetBottomRight());

	spaceShipLight->InitWindowDependentProperties(m_renderTargetSize);


	background->SetWindowSize(m_windowBounds);

	background->SetTexture(m_background);
	background->InitSliding();
}

void SpriteGame::Update(float timeTotal, float timeDelta)
{


	background->Update(timeDelta);
	spaceship->Update(timeDelta);



	rocketFuel->Update(timeDelta);
	for (auto asteroid = m_asteroidData.begin(); asteroid != m_asteroidData.end(); asteroid++)
	{
		asteroid->Update(timeDelta);

		if (asteroid->lifeTime > 0)
			asteroid->lifeTime--;

		if (asteroid->lifeTime == 0)
		{

			asteroid = m_asteroidData.erase(asteroid);

			if (asteroid == m_asteroidData.end())
				break;
			continue;
		}
		if (dist(spaceship->GetPos(), asteroid->GetPos()) < spaceship->textureSize.Width*spaceship->GetScale().x*2.0)
		{
			if (PolygonCollision(spaceship->getCollisionGeometry(), asteroid->getCollisionGeometry()))
			{

				AudioManager::AudioEngineInstance.PlaySoundEffect(Crash);
				asteroid = m_asteroidData.erase(asteroid);

				if (asteroid == m_asteroidData.end())
					break;
				continue;
			}
		}

		for (auto particle = m_particleData.begin(); particle != m_particleData.end(); particle++)
		{
			if (dist(particle->GetPos(), asteroid->GetPos()) < asteroid->textureSize.Width*asteroid->GetScale().x*2.0)
			{
				if (PolygonCollision(particle->getCollisionGeometry(), asteroid->getCollisionGeometry()) && asteroid->lifeTime == -1)
				{

					AudioManager::AudioEngineInstance.PlaySoundEffect(Crash);


					particle = m_particleData.erase(particle);



					for (int i = 0; i < 5; i++)
					{
						Asteroid data;
						data.lifeTime = 20;
						data.SetPos(asteroid->GetPos());
						float tempRot = RandFloat(-PI_F, PI_F);
						float tempMag = RandFloat(60.0f, 80.0f);
						data.SetVel(float2(tempMag * cosf(tempRot), tempMag * sinf(tempRot)));
						data.SetRot(0);
						data.SetScale(asteroid->GetScale() / 4.f);

						data.SetRotVel(RandFloat(-PI_F, PI_F) / (7.0f + 3.0f * asteroid->GetScale().x));
						data.size = (180, 110);
						data.SetTexture(m_asteroid);
						data.SetWindowSize(m_windowBounds);
						m_asteroidFragments.push_back(data);
					}

					asteroid = m_asteroidData.erase(asteroid);

					if (asteroid == m_asteroidData.end())
						break;
					if (particle == m_particleData.end())
						break;

				}
			}
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
	for (auto asteroid = m_asteroidFragments.begin(); asteroid != m_asteroidFragments.end(); asteroid++)
	{
		asteroid->Update(timeDelta);

		if (asteroid->lifeTime > 0)
			asteroid->lifeTime--;

		if (asteroid->lifeTime == 0)
		{

			asteroid = m_asteroidFragments.erase(asteroid);

			if (asteroid == m_asteroidFragments.end())
				break;
			continue;
		}
	}
	spaceShipLight->Update(spaceship);

}

void SpriteGame::Render()
{
	m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);

	m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), reinterpret_cast<float*>(&D2D1::ColorF(D2D1::ColorF::AntiqueWhite)));

	m_spriteBatch->Begin();
	background->Draw(m_spriteBatch);

	for (auto asteroid = m_asteroidData.begin(); asteroid != m_asteroidData.end(); asteroid++)
	{
		asteroid->Draw(m_spriteBatch);
		asteroid->DebugDraw(m_spriteBatch, m_debug_point);

	}
	for (auto asteroid = m_asteroidFragments.begin(); asteroid != m_asteroidFragments.end(); asteroid++)
	{
		asteroid->Draw(m_spriteBatch);

	}
	m_spriteBatch->End();

	spaceShipLight->Draw();
	m_spriteBatch->Begin();

	CollisionGeometry sg = spaceship->getCollisionGeometry();
	for (auto particle = m_particleData.begin(); particle != m_particleData.end(); particle++)
	{
		particle->Draw(m_spriteBatch);


		particle->DebugDraw(m_spriteBatch, m_debug_point);
	}
	spaceship->Draw(m_spriteBatch);

	spaceship->DebugDraw(m_spriteBatch, m_debug_point);
	m_spriteBatch->End();
}

void SpriteGame::CheckScreenType()
{

}




