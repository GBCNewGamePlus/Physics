#pragma once
#include "ECSConfig.h"
#include "RigidBodyComponent.h"
#include "TransformComponentV2.h"
#include "BuoyancyComponent.h"

namespace Reality
{
	class BuoyancyForceGeneratorSystem : public ECSSystem
	{
	private:
		float localZ[4];
		int pointer = 0;
	public:
		BuoyancyForceGeneratorSystem();
		void Update(float deltaTime);
	};
}
