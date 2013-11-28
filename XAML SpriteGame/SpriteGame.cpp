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

}

void SpriteGame::CreateProjectile()
{
	AudioManager::AudioEngineInstance.StopSoundEffect(Shoot);
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

	spaceship->health = 100;
	level->SetWindowDependentProperties(m_windowBounds);
	time_passed = 0;
	score = 0;
}

void SpriteGame::CreateDeviceResources()
{
	DirectXBase::CreateDeviceResources();

	m_spriteBatch = ref new SpriteBatch();
	unsigned int capacity = 60000;

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
	spaceship->SetScale(float2(.35, .35));

	spaceship->_projectile = m_particle;


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
	 

}

void SpriteGame::Update(float timeTotal, float timeDelta)
{
	time_passed += timeDelta;

	level->background->Update(timeDelta);
	level->foreground->Update(timeDelta);

	for (auto object = level->all_gameobjects.begin(); object != level->all_gameobjects.end(); object++) // update level objects
		(*object)->Update(timeDelta);

	for (auto object = m_explosionData.begin(); object != m_explosionData.end(); object++) // update explosions
		object->Update(timeDelta);


	if (gamestate == GameState::Playing) //if playing , create new level objects
		level->Update(timeTotal, timeDelta, m_windowBounds);

	spaceship->Update(timeDelta);

	DetectCollisions();
	HandleCollisions();
	RemoveDead();
	 

}
void SpriteGame::DetectCollisions(){

	colliding.clear();
	for (auto pobject = level->all_gameobjects.begin(); pobject != level->all_gameobjects.end(); pobject++)  // process all player collisions
	{
		if (dynamic_cast<Enemy*>((*pobject)) != NULL) // if it is an enemy we need to add the bullets 
		{
			Enemy* en = dynamic_cast<Enemy*>((*pobject));
			for (auto bullet = en->bullets.begin(); bullet != en->bullets.end(); bullet++) // add enemy bullets to the collision list
			{

				if (dist(bullet->GetPos(), spaceship->GetPos()) < spaceship->textureSize.Width*spaceship->GetScale().x*2.0) // prune collision list based on distance
				{
					if (PolygonCollision(bullet->getCollisionGeometry(), spaceship->getCollisionGeometry())) //checkCollision
						colliding.push_back(pair<GameObject*, GameObject*>(spaceship, &(*bullet)));
				}
			}
		}
		if (dist((*pobject)->GetPos(), spaceship->GetPos()) < spaceship->textureSize.Width*spaceship->GetScale().x*2.0) // prune collision list based on distance
		{
			if (PolygonCollision((*pobject)->getCollisionGeometry(), spaceship->getCollisionGeometry())) //checkCollision
				colliding.push_back(pair<GameObject*, GameObject*>(spaceship, *pobject));

		}
	}

	for (auto pobject = level->all_gameobjects.begin(); pobject != level->all_gameobjects.end(); pobject++) // process all enemy collisions
	{
		for (auto bullet = spaceship->bullets.begin(); bullet != spaceship->bullets.end(); bullet++) //against player bullets
		{
			if ( dynamic_cast<GamePlayElement*>((*pobject)) == NULL )  // We only want to hit Enemies & asteroids, not gameplay stuff
			{
				if (dist((*pobject)->GetPos(), bullet->GetPos()) < (*pobject)->textureSize.Width*(*pobject)->GetScale().x*2.0) // prune collision list based on distance
				{
					if (PolygonCollision((*pobject)->getCollisionGeometry(), bullet->getCollisionGeometry())) //checkCollision
						colliding.push_back(pair<GameObject*, GameObject*>( &(*bullet), *pobject));

				}
			}
		}
	}

}
void SpriteGame::HandleCollisions()
{
	for (auto c = colliding.begin(); c != colliding.end(); c++)
	{

		ImpactResult res1 = c->first->ProcessHit(c->second->GetImpactSize());

		ImpactResult res2 = c->second->ProcessHit(c->first->GetImpactSize());

		if (res1 == ImpactResult::score || res2 == ImpactResult::score)
			score += 1;

		if (res1 == ImpactResult::explosion)
		{
			Explosion data;
			data.lifetime = 30;
			data.SetPos(c->first->GetPos());
			data.SetScale(float2(20.0f, 20.0f));
			data.SetTexture(m_particle);
			data.SetWindowSize(m_windowBounds);
			m_explosionData.push_back(data);
		}
		if (res2 == ImpactResult::explosion)
		{
			Explosion data;
			data.lifetime = 30;
			data.SetPos(c->first->GetPos());
			data.SetScale(float2(20.0f, 20.0f));
			data.SetTexture(m_particle);
			data.SetWindowSize(m_windowBounds);
			m_explosionData.push_back(data);
		}
	}

}
void SpriteGame::RemoveDead()
{
	for (auto pobject = level->all_gameobjects.begin(); pobject != level->all_gameobjects.end(); pobject++)
	{
		if (!(*pobject)->IsAlive())
		{
			delete *(pobject);
			pobject = level->all_gameobjects.erase(pobject);

			if (pobject == level->all_gameobjects.end())
				break;
		}
	}
}

void SpriteGame::Render()
{
	m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);

	m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), reinterpret_cast<float*>(&D2D1::ColorF(D2D1::ColorF::AntiqueWhite)));

	m_spriteBatch->Begin();
	level->background->Draw(m_spriteBatch);
	level->foreground->Draw(m_spriteBatch);

	for (auto object = level->all_gameobjects.begin(); object != level->all_gameobjects.end(); object++)
			(*object)->Draw(m_spriteBatch);
 
	for (auto particle = m_explosionData.begin(); particle != m_explosionData.end(); particle++)
			particle->Draw(m_spriteBatch);
 
	spaceship->Draw(m_spriteBatch);
	 
	m_spriteBatch->End();
}





