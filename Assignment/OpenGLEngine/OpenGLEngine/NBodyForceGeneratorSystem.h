#pragma once
#include "ECSConfig.h"
#include "ParticleComponent.h"
#include "TransformComponent.h"
#include "NBodyComponent.h"

namespace Reality
{
	class NBodyForceGeneratorSystem : public ECSSystem
	{
	public:
		NBodyForceGeneratorSystem();
		void Update(float deltaTime);
	};
}
