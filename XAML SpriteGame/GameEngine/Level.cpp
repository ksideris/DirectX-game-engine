#include "pch.h"
#include "Level.h"




//related to xml
#include "IrrXML\irrXML.h"
using namespace irr;
using namespace io;
//end xml imports

int StringToWString(std::wstring &ws, const std::string &s)
{
	std::wstring wsTmp(s.begin(), s.end());

	ws = wsTmp;

	return 0;
}
void Level::Load(std::string level_loc, BasicSprites::SpriteBatch^ m_spriteBatch, BasicLoader^ loader)
{

	//xml testing
	IrrXMLReader* xml = createIrrXMLReader(level_loc.c_str());

	// strings for storing the data we want to get out of the file
	std::string stringdata2;
	std::string stringdata;
	std::wstring wstringdata;
	Platform::String^ final_data;


	// parse the file until end reached
	while (xml && xml->read())
	{
		switch (xml->getNodeType())
		{

		case EXN_ELEMENT:
			{
				if (!strcmp("Background", xml->getNodeName()))
				{
					stringdata = xml->getAttributeValue("Texture");

					StringToWString(wstringdata, stringdata);


					final_data = L"Assets\\GameObjects\\" + ref new Platform::String(wstringdata.c_str());



					loader->LoadTexture(
						final_data,
						&m_background,
						nullptr
						);
					m_spriteBatch->AddTexture(m_background.Get());



					background = new SlidingBackgroundSprite(BackgroundRenderMode::STRETCH_HEIGHT);
					background->SetTexture(m_background);

				}
				if (!strcmp("Projectile", xml->getNodeName()))
				{
					stringdata = xml->getAttributeValue("Texture");

					StringToWString(wstringdata, stringdata);


					final_data = L"Assets\\GameObjects\\" + ref new Platform::String(wstringdata.c_str());



					loader->LoadTexture(
						final_data,
						&_projectile,
						nullptr
						);
					m_spriteBatch->AddTexture(_projectile.Get());



				}

				if (!strcmp("Foreground", xml->getNodeName()))
				{
					stringdata = xml->getAttributeValue("Texture");
					StringToWString(wstringdata, stringdata);

					final_data = L"Assets\\GameObjects\\" + ref new Platform::String(wstringdata.c_str());


					loader->LoadTexture(
						final_data,
						&m_foreground,
						nullptr
						);
					m_spriteBatch->AddTexture(m_foreground.Get());


					stringdata = xml->getAttributeValue("Texture2");
					StringToWString(wstringdata, stringdata);

					final_data = L"Assets\\GameObjects\\" + ref new Platform::String(wstringdata.c_str());


					loader->LoadTexture(
						final_data,
						&m_foreground2,
						nullptr
						);
					m_spriteBatch->AddTexture(m_foreground2.Get());

					foreground = new SlidingBackgroundSprite(BackgroundRenderMode::STRETCH_HEIGHT);
					foreground->SetTextures(m_foreground, m_foreground2);
					foreground->SetVel(float2(-20, 0));
				}
				if (!strcmp("Asteroid", xml->getNodeName()))
				{
					stringdata = xml->getAttributeValue("Texture");

					StringToWString(wstringdata, stringdata);


					final_data = L"Assets\\GameObjects\\" + ref new Platform::String(wstringdata.c_str());


					loader->LoadTexture(
						final_data,
						&m_asteroid,
						nullptr
						);
					m_spriteBatch->AddTexture(m_asteroid.Get());


				}
				if (!strcmp("RingT", xml->getNodeName()))
				{
					stringdata = xml->getAttributeValue("Texture");

					StringToWString(wstringdata, stringdata);


					final_data = L"Assets\\GameObjects\\" + ref new Platform::String(wstringdata.c_str());


					loader->LoadTexture(
						final_data,
						&m_ring,
						nullptr
						);
					m_spriteBatch->AddTexture(m_ring.Get());


				}
				if (!strcmp("AsteroidField", xml->getNodeName()))
				{
					stringdata = xml->getAttributeValue("Pos");
					stringdata2 = xml->getAttributeValue("Count");

					AsteroidFields.push_back(std::pair<int, float>(atoi(stringdata2.c_str()), (float) atof(stringdata.c_str())));


				}
				if (!strcmp("Ring", xml->getNodeName()))
				{
					stringdata = xml->getAttributeValue("Pos");
					stringdata2 = xml->getAttributeValue("Count");

					Rings.push_back(std::pair<int, float>(atoi(stringdata2.c_str()), (float) atof(stringdata.c_str())));


				}
				if (!strcmp("EnemyType", xml->getNodeName()))
				{
					stringdata2 = xml->getAttributeValue("Texture");

					StringToWString(wstringdata, stringdata2);


					final_data = L"Assets\\GameObjects\\" + ref new Platform::String(wstringdata.c_str());
					Microsoft::WRL::ComPtr<ID3D11Texture2D> etexture;


					loader->LoadTexture(
						final_data,
						&etexture,
						nullptr
						);
					m_spriteBatch->AddTexture(etexture.Get());



					EnemyTextures.push_back(etexture);

				}
				if (!strcmp("Enemy", xml->getNodeName()))
				{
					stringdata2 = xml->getAttributeValue("Movem");
					stringdata = xml->getAttributeValue("Type");
					std::pair<int, int> attributes = std::pair<int, int>(atoi(stringdata2.c_str()), atoi(stringdata.c_str()));

					stringdata = xml->getAttributeValue("Pos");
					Enemies.push_back(std::pair<  std::pair<int, int>, float>(attributes, (float) atof(stringdata.c_str())));


				}
				break;

						}
		}
	}

	// delete the xml parser after usage
	delete xml;

	///end xml testing
}

void Level::SetWindowDependentProperties(Windows::Foundation::Rect  m_windowBounds){
	background->SetWindowSize(m_windowBounds);
	background->InitSliding();
	foreground->SetWindowSize(m_windowBounds);
	foreground->InitSliding();

}

void Level::Update(float timeTotal, float timeDelta, Windows::Foundation::Rect  m_windowBounds)
{

	for (auto field = AsteroidFields.begin(); field != AsteroidFields.end(); field++)
	{
		if (timeTotal > field->second   && timeTotal < field->second + timeDelta)
		{
			for (int i = 0; i < field->first; i++)
			{
				PassiveObject data;
				data.type = PassiveObjectType::ASTEROID;

				data.SetPos(float2(m_windowBounds.Width + 200, RandFloat(0.0f, m_windowBounds.Height)));
				float tempRot = RandFloat(-PI_F, PI_F);
				float tempMag = RandFloat(60.0f, 100.0f);
				float tempScale = RandFloat(0.1f, 1.0f);

				data.SetVel(float2(-tempMag, 0.f));
				data.SetRot(0);
				data.SetScale(float2(tempScale, tempScale));
				data.SetRotVel(RandFloat(-PI_F, PI_F) / (7.0f + 3.0f * tempScale));
				data.SetTexture(m_asteroid);
				data.SetWindowSize(m_windowBounds);


				data.lifeTime = -1;
				passive_objects.push_back(data);
			}
		}
	}
	for (auto ring = Rings.begin(); ring != Rings.end(); ring++)
	{
		if (timeTotal > ring->second   && timeTotal < ring->second + timeDelta)
		{
			for (int i = 0; i < ring->first; i++)
			{
				PassiveObject data;
				data.type = PassiveObjectType::RING;

				data.SetPos(float2(m_windowBounds.Width + 200, RandFloat(m_windowBounds.Height / 4, 3 * m_windowBounds.Height / 4)));
				float tempRot = RandFloat(-PI_F, PI_F);
				float tempMag = RandFloat(60.0f, 100.0f);

				data.SetVel(float2(-tempMag, 0.f));
				data.SetRot(tempRot);
				data.SetScale(float2(.15f, .15f));
				data.SetRotVel(0);
				data.SetTexture(m_ring);
				data.SetWindowSize(m_windowBounds);


				data.lifeTime = -1;
				passive_objects.push_back(data);
			}
		}
	}
	for (auto enemy = Enemies.begin(); enemy != Enemies.end(); enemy++)
	{
		if (timeTotal > enemy->second   && timeTotal < enemy->second + timeDelta)
		{
			(enemy->first).first;
			(enemy->first).second;
			Enemy data(EnemyMovement((enemy->first).second));

			data.SetPos(float2(m_windowBounds.Width + 200, RandFloat(m_windowBounds.Height / 4, 3 * m_windowBounds.Height / 4)));


			data.SetVel(float2(0.f, 0.f));
			data._projectile = _projectile;
			data.SetScale(float2(.75f, .75f));
			data.SetRotVel(0);
			data.SetTarget(data.GetPos());
			data.SetTexture(EnemyTextures[(enemy->first).first]);
			data.SetWindowSize(m_windowBounds);


			enemies.push_back(data);

		}
	}
}