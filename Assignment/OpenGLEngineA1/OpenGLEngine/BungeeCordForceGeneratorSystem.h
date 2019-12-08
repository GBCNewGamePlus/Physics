#pragma once
#include "ECSConfig.h"
#include "ParticleComponent.h"
#include "TransformComponent.h"
#include "BungeeCordComponent.h"

namespace Reality
{
	class BungeeCordForceGeneratorSystem : public ECSSystem
	{
	public:
		BungeeCordForceGeneratorSystem();
		void Update(float deltaTime);
	};
}
