#pragma once
#include "ECSConfig.h"

namespace Reality
{
	struct WaterComponent
	{
		WaterComponent(float _waterDepth = 10.0f)
		{
			waterDepth = _waterDepth;
		}
		float waterDepth;
	};
}
