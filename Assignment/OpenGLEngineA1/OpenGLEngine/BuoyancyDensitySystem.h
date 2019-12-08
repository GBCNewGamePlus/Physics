#pragma once
#include "ECSConfig.h"
#include "BuoyancySpawnComponent.h"

namespace Reality
{
	class BuoyancyDensitySystem : public ECSSystem
	{
	private:
		bool keyReleased = true;
		int keyUsed = 0;
	public:
		BuoyancyDensitySystem();
		void Update(float deltaTime);
	};
}

