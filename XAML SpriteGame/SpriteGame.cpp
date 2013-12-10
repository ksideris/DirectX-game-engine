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
	GlobalHelper::getData()->time_passed = 0;
}

void SpriteGame::CreateDeviceIndependentResources()
{
	DirectXBase::CreateDeviceIndependentResources();

}

void SpriteGame::LoadLevel(Platform::String^ level_xml)
{
	GlobalData* gdata = GlobalHelper::getData();

	if (gdata->level != NULL)
	{
		delete gdata->level;
	}

	BasicLoader^ loader = ref new BasicLoader(m_d3dDevice.Get(), m_wicFactory.Get());
	gdata->level = new Level();
	std::wstring level_xmlW(level_xml->Begin());
	std::string level_xmlA(level_xmlW.begin(), level_xmlW.end());
	gdata->level->Load("Level Data\\" + level_xmlA, m_spriteBatch, loader);

	gdata->spaceship->health = 100;
	gdata->spaceship->Reset();

	gdata->spaceship->SetTarget(float2(0.f, m_windowBounds.Height / 2.f));
	gdata->level->SetWindowDependentProperties(m_windowBounds);
	 
	gdata->time_passed = 0;
	gdata->score = 0;
	last_explosion = 0;
}

void SpriteGame::CreateDeviceResources()
{
	DirectXBase::CreateDeviceResources();

	GlobalData* gdata = GlobalHelper::getData();

	m_spriteBatch = ref new SpriteBatch();
	unsigned int capacity = 15000;

	m_spriteBatch->Initialize(
		m_d3dDevice.Get(),
		capacity
		);

	// Load the sprite textures.

	BasicLoader^ loader = ref new BasicLoader(m_d3dDevice.Get(), m_wicFactory.Get());


	loader->LoadTexture(
		"Assets\\GameObjects\\spotlight.png",
		&gdata->m_player_spot,
		nullptr
		);

	m_spriteBatch->AddTexture(gdata->m_player_spot.Get());

	loader->LoadTexture(
		"Assets\\GameObjects\\ida.dds",
		&gdata->m_asteroid,
		nullptr
		);
	m_spriteBatch->AddTexture(gdata->m_asteroid.Get());

	loader->LoadTexture(
		"Assets\\GameObjects\\SpaceShip.png",
		&gdata->m_player,
		nullptr
		);
	m_spriteBatch->AddTexture(gdata->m_player.Get());

	loader->LoadTexture(
		"Assets\\GameObjects\\particle.png",
		&gdata->m_particle,
		nullptr
		);
	m_spriteBatch->AddTexture(gdata->m_particle.Get());

	loader->LoadTexture(
		"Assets\\GameObjects\\debug.png",
		&gdata->m_debug_point,
		nullptr
		);
	m_spriteBatch->AddTexture(gdata->m_debug_point.Get());
	loader->LoadTexture(
		"Assets\\GameObjects\\bad_health.png",
		&gdata->m_background,
		nullptr
		);
	m_spriteBatch->AddTexture(gdata->m_background.Get());
	 


	gdata->spaceship = new HorizontalSliderPlayer();
	gdata->rocketFuel = new RocketFire();

	gdata->spaceship->SetVel(float2(0, 0));
	gdata->spaceship->SetRot(0);
	gdata->spaceship->SetRotVel(0);
	gdata->spaceship->SetScale(float2(.35, .35));

	gdata->spaceship->_projectile = gdata->m_particle;

	gdata->bad_health_background = new FlashingBackground();
	gdata->bad_health_background->SetTexture(gdata->m_background);



}

void SpriteGame::CreateWindowSizeDependentResources()
{
	DirectXBase::CreateWindowSizeDependentResources();

	GlobalData* gdata = GlobalHelper::getData();

	gdata->spaceship->SetWindowSize(m_windowBounds);

	gdata->spaceship->SetPos(float2(0.f, m_windowBounds.Height / 2.0f));

	gdata->spaceship->SetTexture(gdata->m_player);
	gdata->rocketFuel->SetTexture(gdata->m_particle);
	gdata->spaceship->setForwardTriangleCollisionGeometry(gdata->spaceship->GetTopLeft(), gdata->spaceship->GetBottomRight());
	gdata->spaceship->AddChild(float2(gdata->spaceship->textureSize.Width / 2.0f*gdata->spaceship->GetScale().x, gdata->spaceship->textureSize.Height / 2.0f*gdata->spaceship->GetScale().y), gdata->rocketFuel);

	gdata->rocketFuel->SetPos(gdata->spaceship->GetPos());
	 
	if (gdata->level != NULL)
		gdata->level->background->SetWindowSize(m_windowBounds);


 
	gdata->bad_health_background->SetWindowSize(m_windowBounds);

}

void SpriteGame::Update(float timeTotal, float timeDelta)
{
	GlobalData* gdata = GlobalHelper::getData();
	gdata->time_passed += timeDelta;

	gdata->level->background->Update(timeDelta);
	gdata->level->foreground->Update(timeDelta);
	gdata->bad_health_background->Update(timeDelta);
	gdata->spaceship->Update(timeDelta);

	for (auto object = gdata->level->all_gameobjects.begin(); object != gdata->level->all_gameobjects.end(); object++) // update level objects
		(*object)->Update(timeDelta);

	for (auto object = gdata->m_explosionData.begin(); object != gdata->m_explosionData.end(); object++) // update explosions
		object->Update(timeDelta);

	if (gamestate == GameState::LevelWon &&   timeTotal - last_explosion > .1)
	{
		Explosion data;
		data.SetLifeTime(100);
		data.SetPos(float2(RandFloat(0, m_windowBounds.Width), RandFloat(0, m_windowBounds.Height)));
		data.color = float4(RandFloat(0, 1), RandFloat(0, 1), RandFloat(0, 1), 1.f);
		data.SetScale(float2(30.0f, 30.0f));
		data.SetTexture(gdata->m_particle);
		data.SetWindowSize(m_windowBounds);
		gdata->m_explosionData.push_back(data);
		last_explosion = timeTotal;
	}
	if (gamestate == GameState::LevelLost &&   timeTotal - last_explosion > .3)
	{
		Explosion data;
		data.SetLifeTime(100);
		data.SetPos(gdata->spaceship->GetPos());
		data.color = float4(1.f, 0.3f, .0f, 1.f);
		data.SetScale(float2(30.0f, 30.0f));
		data.SetTexture(gdata->m_particle);
		data.SetWindowSize(m_windowBounds);
		gdata->m_explosionData.push_back(data);
		last_explosion = timeTotal;
	}

	if (gamestate == GameState::Playing) //if playing , create new level objects
	{
		gdata->level->Update(timeTotal, timeDelta, m_windowBounds);

		DetectCollisions();
		HandleCollisions();

		if (!gdata->spaceship->IsAlive())
			gamestate = GameState::GameOver;
	}

	RemoveDead();

}
void SpriteGame::DetectCollisions(){

	GlobalData* gdata = GlobalHelper::getData();

	colliding.clear();
	for (auto pobject = gdata->level->all_gameobjects.begin(); pobject != gdata->level->all_gameobjects.end(); pobject++)  // process all player collisions
	{
		if (dynamic_cast<Enemy*>((*pobject)) != NULL) // if it is an enemy we need to add the bullets 
		{
			Enemy* en = dynamic_cast<Enemy*>((*pobject));
			for (auto bullet = en->bullets.begin(); bullet != en->bullets.end(); bullet++) // add enemy bullets to the collision list
			{

				if (dist(bullet->GetPos(), gdata->spaceship->GetPos()) < gdata->spaceship->textureSize.Width*gdata->spaceship->GetScale().x*2.0) // prune collision list based on distance
				{
					if (PolygonCollision(bullet->getCollisionGeometry(), gdata->spaceship->getCollisionGeometry())) //checkCollision
						colliding.push_back(pair<GameObject*, GameObject*>(gdata->spaceship, &(*bullet)));
				}
			}
		}
		if (dist((*pobject)->GetPos(), gdata->spaceship->GetPos()) < gdata->spaceship->textureSize.Width*gdata->spaceship->GetScale().x*2.0) // prune collision list based on distance
		{
			if (PolygonCollision((*pobject)->getCollisionGeometry(), gdata->spaceship->getCollisionGeometry())) //checkCollision
				colliding.push_back(pair<GameObject*, GameObject*>(gdata->spaceship, *pobject));

		}
	}

	for (auto pobject = gdata->level->all_gameobjects.begin(); pobject != gdata->level->all_gameobjects.end(); pobject++) // process all enemy collisions
	{
		for (auto bullet = gdata->spaceship->bullets.begin(); bullet != gdata->spaceship->bullets.end(); bullet++) //against player bullets
		{
			if (dynamic_cast<GamePlayElement*>((*pobject)) == NULL)  // We only want to hit Enemies & asteroids, not gameplay stuff
			{
				if (dist((*pobject)->GetPos(), bullet->GetPos()) < (*pobject)->textureSize.Width*(*pobject)->GetScale().x*2.0) // prune collision list based on distance
				{
					if (PolygonCollision((*pobject)->getCollisionGeometry(), bullet->getCollisionGeometry())) //checkCollision
						colliding.push_back(pair<GameObject*, GameObject*>(&(*bullet),*pobject));

				}
			}
		}
	}

}
void SpriteGame::HandleCollisions()
{
	GlobalData* gdata = GlobalHelper::getData();

	for (auto c = colliding.begin(); c != colliding.end(); c++)
	{

		ImpactResult res1 = c->first->ProcessHit(c->second->GetImpactSize());

		ImpactResult res2 = c->second->ProcessHit(c->first->GetImpactSize());

		if (res1 == ImpactResult::score || res2 == ImpactResult::score)
			gdata->score += 1;

		if (res1 == ImpactResult::explosion || res1 == ImpactResult::bigexplosion  )
		{
			Explosion data;
			data.SetPos(c->first->GetPos());
			

			if (res1 == ImpactResult::explosion  )
			{
				data.SetScale(float2(20.f, 20.f));
				data.SetLifeTime(10);
			}
			else
			{
				data.SetScale(float2(30.f, 30.f));
				data.SetLifeTime(100);
			}
			data.SetTexture(gdata->m_particle);
			data.SetWindowSize(m_windowBounds);
			gdata->m_explosionData.push_back(data);
		}

		if (res2 == ImpactResult::explosion || res2 == ImpactResult::bigexplosion  )
		{
			Explosion data;
			data.SetPos(c->first->GetPos());


			if (res2 == ImpactResult::explosion)
			{
				data.SetScale(float2(20.f, 20.f));
				data.SetLifeTime(10);
			}
			else
			{
				data.SetScale(float2(30.f, 30.f));
				data.SetLifeTime(100);
			}
			data.SetTexture(gdata->m_particle);
			data.SetWindowSize(m_windowBounds);
			gdata->m_explosionData.push_back(data);
		}

		if (res1 == ImpactResult::finish || res2 == ImpactResult::finish)
		{
			gamestate = GameState::LevelComplete;
		}
		if (res1 == ImpactResult::health || res2 == ImpactResult::health)
		{
			gdata->spaceship->health = 100;
		}
		if (res1 == ImpactResult::weapon_upgrade || res2 == ImpactResult::weapon_upgrade)
		{
			gdata->spaceship->UpgradeWeapons();
		}
	}

}
void SpriteGame::RemoveDead()
{
	GlobalData* gdata = GlobalHelper::getData();

	for (auto pobject = gdata->level->all_gameobjects.begin(); pobject != gdata->level->all_gameobjects.end(); pobject++)
	{
		if (!(*pobject)->IsAlive())
		{
			delete *(pobject);
			pobject = gdata->level->all_gameobjects.erase(pobject);

			if (pobject == gdata->level->all_gameobjects.end())
				break;
		}
	}

	for (auto pobject = gdata->m_explosionData.begin(); pobject != gdata->m_explosionData.end(); pobject++)
	{
		if (!pobject->IsAlive())
		{
			pobject = gdata->m_explosionData.erase(pobject);

			if (pobject == gdata->m_explosionData.end())
				break;
		}
	}
}

void SpriteGame::Render()
{
	GlobalData* gdata = GlobalHelper::getData();

	m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);

	m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), reinterpret_cast<float*>(&D2D1::ColorF(D2D1::ColorF::AntiqueWhite)));

	m_spriteBatch->Begin();

	gdata->level->background->Draw(m_spriteBatch);
	gdata->level->foreground->Draw(m_spriteBatch);

	for (auto object = gdata->level->all_gameobjects.begin(); object != gdata->level->all_gameobjects.end(); object++)
		(*object)->Draw(m_spriteBatch);
	
	if (gdata->spaceship->IsAlive())
		gdata->spaceship->Draw(m_spriteBatch);

	if (gdata->spaceship->health < 30 && gdata->spaceship->health>0 && gamestate == GameState::Playing)
		gdata->bad_health_background->Draw(m_spriteBatch);


	for (auto particle = gdata->m_explosionData.begin(); particle != gdata->m_explosionData.end(); particle++)
		particle->Draw(m_spriteBatch);

	m_spriteBatch->End();
}





