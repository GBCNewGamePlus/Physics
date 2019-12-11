#pragma once
#include "ECSConfig.h"

namespace Reality
{
	class WaterSystem : public ECSSystem
	{
	public:
		WaterSystem();
		void Update(float deltaTime);
	};
}
