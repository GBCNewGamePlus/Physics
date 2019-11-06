#pragma once
#include "ECSConfig.h"
#include "SphereComponent.h"
#include "TransformComponent.h"

namespace Reality 
{
	class SphereContactSystem: public ECSSystem
	{
	public:
		SphereContactSystem();
		void Update(float deltaTime);
	};

}

