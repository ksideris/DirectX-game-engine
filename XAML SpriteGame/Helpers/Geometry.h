#pragma once
#include "pch.h"


namespace Helpers
{
	ref class Geometry abstract
	{
	internal:
		// We need the xMargin and yMargin here because some objects might be relatively 
		// large when we are comparing the origin (central point) to the location of another 
		// origin. Ommiting this fact *FAILS THE COLLISION CHECK* for large objects.

		static bool IsInProximity(float2 primaryPosition, float2 secondaryPosition, float threshold)
		{
			float xDiff = abs(primaryPosition.x - secondaryPosition.x);
			float yDiff = abs(primaryPosition.y - secondaryPosition.y);

			float distance = sqrt(xDiff*xDiff + yDiff*yDiff);

			if (distance > threshold)
				return false;
			else
				return true;
		}

		static Windows::Foundation::Rect MoveRectangle(Windows::Foundation::Rect sourceRectangle, float x, float y)
		{
			return Windows::Foundation::Rect(sourceRectangle.X + x, sourceRectangle.Y + y,
				sourceRectangle.Width, sourceRectangle.Height);
		}
	};
}
