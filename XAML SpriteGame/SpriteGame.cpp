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
	gamestate = Menu;
	time_passed = 0;
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
void SpriteGame::CreateEnemyProjectile(Enemy* enemy)
{
	FireBall data;
	data.SetPos(enemy->GetPos());
	data.vel = float2(1000.0f* cos(enemy->GetRot()), -1000.0f*sin(enemy->GetRot()));
	data.SetScale(float2(1.0f, 1.0f));
	data.SetTexture(m_particle);
	data.setCollisionGeometryForParticle(float2(20, 20), data.GetPos());
	data.SetWindowSize(m_windowBounds);
	m_particleData.push_back(data);
	AudioManager::AudioEngineInstance.PlaySoundEffect(Shoot);
}
void SpriteGame::LoadLevel(Platform::String^ level_xml)
{
	if (level != NULL)
	{
		delete level;
	}

	BasicLoader^ loader = ref new BasicLoader(m_d3dDevice.Get(), m_wicFactory.Get());
	level = new Level();
	std::wstring level_xmlW(level_xml->Begin());
	std::string level_xmlA(level_xmlW.begin(), level_xmlW.end());
	level->Load("Level Data\\" + level_xmlA, m_spriteBatch, loader);


	level->background->SetWindowSize(m_windowBounds);
	level->background->InitSliding();
	time_passed = 0;
	score = 0;
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


	spaceship = new HorizontalSliderPlayer();
	rocketFuel = new RocketFire();

	//spaceShipLight = new d2dLightEffect();
	//spaceShipLight->initWithImageFromFile(m_d2dContext, m_wicFactory);

	spaceship->SetVel(float2(0, 0));
	spaceship->SetRot(0);
	spaceship->SetRotVel(0);
	spaceship->SetScale(float2(.5, .5));


}

void SpriteGame::CreateWindowSizeDependentResources()
{
	DirectXBase::CreateWindowSizeDependentResources();


	spaceship->SetWindowSize(m_windowBounds);

	spaceship->SetPos(float2(0.f, m_windowBounds.Height / 2.0f));

	spaceship->SetTexture(m_player);
	rocketFuel->SetTexture(m_particle);
	spaceship->setForwardTriangleCollisionGeometry(spaceship->GetTopLeft(), spaceship->GetBottomRight());
	spaceship->AddChild(float2(spaceship->textureSize.Width / 2.0f*spaceship->GetScale().x, spaceship->textureSize.Height / 2.0f*spaceship->GetScale().y), rocketFuel);



	rocketFuel->SetPos(spaceship->GetPos());

	if (level != NULL)
	{
		level->background->SetWindowSize(m_windowBounds);
	}

	//spaceShipLight->InitWindowDependentProperties(m_renderTargetSize);

	//level->ground->SetWindowSize(m_windowBounds);


}

void SpriteGame::Update(float timeTotal, float timeDelta)
{
	time_passed += timeDelta;

	level->background->Update(timeDelta);
	level->Update(timeTotal, timeDelta, m_windowBounds);

	spaceship->Update(timeDelta);
	rocketFuel->Update(timeDelta);


	for (auto pobject = level->passive_objects.begin(); pobject != level->passive_objects.end(); pobject++)
	{
		pobject->Update(timeDelta);


		if (pobject->lifeTime > 0)
			pobject->lifeTime--;

		if (pobject->lifeTime == 0 || pobject->GetPos().x < -200)
		{
			pobject->dead = true;

		}
		if (gamestate == Playing && dist(spaceship->GetPos(), pobject->GetPos()) < spaceship->textureSize.Width*spaceship->GetScale().x*2.0)
		{
			if ((pobject->type == PassiveObjectType::RING || pobject->type == PassiveObjectType::ASTEROID) && PolygonCollision(spaceship->getCollisionGeometry(), pobject->getCollisionGeometry()))
			{
				if (pobject->type == PassiveObjectType::ASTEROID){
					spaceship->health -= pobject->GetScale().x * 10.f;
				}
				else if (pobject->type == PassiveObjectType::RING )
				{
					score++;
				}
				AudioManager::AudioEngineInstance.StopSoundEffect(Crash);
				AudioManager::AudioEngineInstance.PlaySoundEffect(Crash);

				pobject->dead = true;
			}
		}

		for (auto particle = m_particleData.begin(); particle != m_particleData.end(); particle++)
		{
			if (gamestate == Playing &&  dist(particle->GetPos(), pobject->GetPos()) < pobject->textureSize.Width*pobject->GetScale().x*2.0)
			{
				if (pobject->lifeTime == -1 && pobject->type == PassiveObjectType::ASTEROID && PolygonCollision(particle->getCollisionGeometry(), pobject->getCollisionGeometry()))
				{

					AudioManager::AudioEngineInstance.PlaySoundEffect(Crash);

					particle = m_particleData.erase(particle);



					for (int i = 0; i < 5; i++)
					{
						PassiveObject  data;
						data.lifeTime = 20;
						data.type = PassiveObjectType::FRAGMENT;
						data.SetPos(pobject->GetPos());
						float tempRot = RandFloat(-PI_F, PI_F);
						float tempMag = RandFloat(60.0f, 80.0f);
						data.SetVel(float2(tempMag * cosf(tempRot), tempMag * sinf(tempRot)));
						data.SetRot(0);
						data.SetScale(pobject->GetScale() / 4.f);

						data.SetRotVel(RandFloat(-PI_F, PI_F) / (7.0f + 3.0f * pobject->GetScale().x));
						data.size = (180.f, 110.f);
						data.SetTexture(m_asteroid);
						data.SetWindowSize(m_windowBounds);
						m_asteroidFragments.push_back(data);
					}


					pobject->dead = true;

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
	//spaceShipLight->Update(spaceship);

	for (int i = 0; i < level->passive_objects.size(); i++)
	{
		if (level->passive_objects[i].dead)
			level->passive_objects.erase(level->passive_objects.begin() + i);
	}



}

void SpriteGame::Render()
{
	m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);

	m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), reinterpret_cast<float*>(&D2D1::ColorF(D2D1::ColorF::AntiqueWhite)));

	m_spriteBatch->Begin();
	level->background->Draw(m_spriteBatch);


	for (auto asteroid = level->passive_objects.begin(); asteroid != level->passive_objects.end(); asteroid++)
	{
		asteroid->Draw(m_spriteBatch);
		//asteroid->DebugDraw(m_spriteBatch, m_debug_point);

	}
	for (auto asteroid = m_asteroidFragments.begin(); asteroid != m_asteroidFragments.end(); asteroid++)
	{
		asteroid->Draw(m_spriteBatch);

	}
	//m_spriteBatch->End();

	//spaceShipLight->Draw();
	//m_spriteBatch->Begin();

	CollisionGeometry sg = spaceship->getCollisionGeometry();
	for (auto particle = m_particleData.begin(); particle != m_particleData.end(); particle++)
	{
		particle->Draw(m_spriteBatch);


		//particle->DebugDraw(m_spriteBatch, m_debug_point);
	}
	spaceship->Draw(m_spriteBatch);

	//spaceship->DebugDraw(m_spriteBatch, m_debug_point);
	m_spriteBatch->End();
}

void SpriteGame::CheckScreenType()
{

}




