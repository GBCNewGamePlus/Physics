#pragma once
#include "ECSConfig.h"
#include "BuoyancySpawnComponent.h"
#include "BuoyancyComponent.h"
#include "TransformComponent.h"
#include "ParticleComponent.h"
#include "SphereComponent.h"

namespace Reality
{
	class BuoyancySpawnSystem : public ECSSystem
	{
	private:
		bool keyReleased = true;
	public:
		BuoyancySpawnSystem();
		void Update(float deltaTime);
	};
}

