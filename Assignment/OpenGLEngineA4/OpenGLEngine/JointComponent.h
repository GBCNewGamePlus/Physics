#pragma once
#include "ECSConfig.h"
#include "TransformComponentV2.h"

namespace Reality
{
	struct JointComponent
	{
		JointComponent(
			ECSEntity _entityA = ECSEntity(),
			Vector3 _localSocketA = Vector3(0, 0, 0),
			ECSEntity _entityB = ECSEntity(),
			Vector3 _localSocketB = Vector3(0, 0, 0),
			float _epsilon = 0.1f) :
			entityA(_entityA),
			localSocketA(_localSocketA),
			entityB(_entityB),
			localSocketB(_localSocketB),
			epsilon(_epsilon)
		{
			if (_entityA != NULL && _entityB != NULL
				&& _entityA.hasComponent<TransformComponentV2>()
				&& _entityB.hasComponent<TransformComponentV2>())
			{
				Vector3 socketAWPosition = _entityA.getComponent<TransformComponentV2>().LocalToWorldPosition(_localSocketA);
				Vector3 socketBWPosition = _entityB.getComponent<TransformComponentV2>().LocalToWorldPosition(_localSocketB);
				initialDistance = glm::length(socketAWPosition - socketBWPosition);
			}
		}
	public:
		ECSEntity entityA;
		Vector3 localSocketA;
		ECSEntity entityB;
		Vector3 localSocketB;
		float epsilon;
		float initialDistance;
	};
}