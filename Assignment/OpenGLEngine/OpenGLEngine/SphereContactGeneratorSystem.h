#pragma once
#include "ECSConfig.h"
#include "SphereComponent.h"
#include "ParticleComponent.h"
#include "TriangleComponent.h"
#include "TransformComponent.h"

namespace Reality
{
	class SphereContactGeneratorSystem : public ECSSystem
	{
	public:
		SphereContactGeneratorSystem();
		void Update(float deltaTime);
	private:
		bool dummyCreated = false;
		ECSEntity dummy;
	};
}

