#pragma once
#include "ECSConfig.h"
#include "RigidBodyComponent.h"

namespace Reality
{
	class GravityForceGeneratorSystem : public ECSSystem
	{
	public:
		Vector3 gravity = Vector3(0, -9.8, 0);
		GravityForceGeneratorSystem();
		void Update(float deltaTime);
	};
}

