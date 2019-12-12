#pragma once
#include "ECSConfig.h"
#include "TransformComponentV2.h"
#include "SphereComponent.h"

namespace Reality
{
	struct JointComponent
	{
		JointComponent(ECSEntity _entityA = ECSEntity(), ECSEntity _entityB = ECSEntity(), Vector3 _pos1 = Vector3(0,0,0), Vector3 _pos2 = Vector3(0,0,0), float _error = 1.0f)
			: entityA(_entityA), entityB(_entityB), pos1(_pos1), pos2(_pos2), error(_error)
		{

		}
		ECSEntity entityA, entityB;
		Vector3 pos1, pos2;
		float error;
		float restitution = 0;
	};
}
