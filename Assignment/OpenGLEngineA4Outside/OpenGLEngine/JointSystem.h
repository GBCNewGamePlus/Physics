#pragma once
#include "ECSConfig.h"
#include "JointComponent.h"

namespace Reality
{
	class JointSystem : public ECSSystem
	{
	public:
		JointSystem();
		void Update(float deltaTime);
	};
}
