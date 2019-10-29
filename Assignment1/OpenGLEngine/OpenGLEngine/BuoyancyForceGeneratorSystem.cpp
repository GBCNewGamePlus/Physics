#include "BuoyancyForceGeneratorSystem.h"

namespace Reality
{
	BuoyancyForceGeneratorSystem::BuoyancyForceGeneratorSystem()
	{
		requireComponent<BuoyancyComponent>();
		requireComponent<SphereComponent>();
	}


	void BuoyancyForceGeneratorSystem::Update(float deltaTime)
	{
		for (auto e : getEntities())
		{
			auto &buoyancy = e.getComponent<BuoyancyComponent>();
			// Verifies "collision" between vat and sphere
			
			// Verifies self-destroy 


		}
	}
}