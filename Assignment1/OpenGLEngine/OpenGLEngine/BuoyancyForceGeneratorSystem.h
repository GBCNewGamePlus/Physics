#pragma once
#include "ECSConfig.h"
#include "ParticleComponent.h"
#include "TransformComponent.h"
#include "BuoyancyComponent.h"
#include "SphereComponent.h"

namespace Reality
{
	class BuoyancyForceGeneratorSystem : public ECSSystem
	{
	public:
		BuoyancyForceGeneratorSystem();
		void Update(float deltaTime);
	};
}
