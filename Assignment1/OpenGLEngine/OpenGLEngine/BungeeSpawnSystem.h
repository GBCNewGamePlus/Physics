#pragma once
#include "ECSConfig.h"
#include "BungeeSpawnComponent.h"
#include "BungeeCordComponent.h"
#include "TransformComponent.h"
#include "ParticleComponent.h"

namespace Reality
{

	class BungeeSpawnSystem : public ECSSystem
	{
	private:
		bool keyReleased = true;
	public:
		BungeeSpawnSystem();
		void Update(float deltaTime);
	};
}

