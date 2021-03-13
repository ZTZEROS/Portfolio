#include "stdafx.h"
#include "utils.h"

namespace ZELING_UTIL
{
	float getDistance(float startX, float startY, float endX, float endY)
	{
		float x = endX - startX;
		float y = endY - startY;

		return sqrtf(x * x + y * y);
	}

	//앵글값을 얻어오는 함수 3D에서 벡터개념과 비슷
	float getAngle(float x1, float y1, float x2, float y2)
	{
		float x = x2 - x1;
		float y = y2 - y1;

		float distance = sqrtf(x * x + y * y);
		float angle = acosf(x / distance);

		if (y2 > y1)
		{
			angle = PI2 - angle;

			if (angle >= PI2) angle -= PI2;
		}

		return angle;
	}

}