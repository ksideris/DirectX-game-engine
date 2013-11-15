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
	std::string modelFile;
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



					background = new SlidingBackgroundSprite();
					background->SetTexture(m_background);

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


				}if (!strcmp("AsteroidField", xml->getNodeName()))
				{
					stringdata = xml->getAttributeValue("Pos");

					AsteroidFields.push_back(std::pair<float, int>(10, atoi(stringdata.c_str())));


				}
				break;

						}
		}
	}

	// delete the xml parser after usage
	delete xml;
	///end xml testing
}


void Level::Update(float timeTotal, float timeDelta, Windows::Foundation::Rect  m_windowBounds)
{

	for (auto field = AsteroidFields.begin(); field != AsteroidFields.end(); field++)
	{
		if (m_asteroidData.size() < 100 && ((int) timeTotal) % field->second == 0 && timeTotal - (int) timeTotal <= timeDelta)
		{
			for (int i = 0; i < field->first; i++)
			{
				Asteroid data;
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


				data.size = float2(180, 110);
				data.lifeTime = -1;
				m_asteroidData.push_back(data);
			}
		}
	}

}