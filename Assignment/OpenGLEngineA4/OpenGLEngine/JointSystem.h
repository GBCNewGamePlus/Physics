#pragma once
#include "ECSConfig.h"
#include "Joint.h"
namespace Reality
{
	class JointSystem : public ECSSystem
	{
	public:
		JointSystem();
		void Update(float deltaTime);
	};
}
