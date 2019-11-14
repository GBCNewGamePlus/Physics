#pragma once
#include "ECSConfig.h"
#include "BungeeSpawnComponent.h"
#include "BungeeCordComponent.h"
#include "TransformComponent.h"
#include "ParticleComponent.h"
#include "SphereComponent.h"

namespace Reality
{
	class SphereSpawnSystem : public ECSSystem
	{
	private:
		bool keyReleased = true;
	public:
		ECSEntity UniqueSphere;
		SphereSpawnSystem();
		void Update(float deltaTime);
	};
}

