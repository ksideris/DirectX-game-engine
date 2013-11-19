#include "pch.h"
#include "Ground.h"


void Ground::Draw(BasicSprites::SpriteBatch^ m_spriteBatch, float CurrLoc)
{

	std::vector < std::pair<float, float> > curView;
	for (auto height = heightMap.begin(); height != heightMap.end(); height++)
	{


		if (height->first >= CurrLoc && height->first <= CurrLoc + _windowRect.Width)
		{

			curView.push_back(std::pair<float, float>(height->first - CurrLoc, height->second));

		}

	}

	for (int i = 0; i < _windowRect.Width; i += 10)
	{
		float h = 0;
		for (auto height = curView.begin(); height != curView.end(); height++)
		{
			if (height->first >= i)
			{

				/*if (height != curView.begin())
				{
				std::pair<float, float> pt1 = curView.at(height - 1);
				std::pair<float, float> pt2 = *height;

					h = (pt2.second + pt1.second) / 2.0f;
				}
				else
				{*/

				std::pair<float, float> pt2 = *height;
					h = pt2.second;
				/*}*/
				break;
			}
		}
		m_spriteBatch->Draw(
			_texture.Get(),
			float2(i, _windowRect.Height),
			PositionUnits::DIPs,
			float2(.1f, h),
			SizeUnits::Normalized
			);

	}


}
