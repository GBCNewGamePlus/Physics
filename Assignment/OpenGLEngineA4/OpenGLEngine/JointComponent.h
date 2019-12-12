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
			float _initialDistance = 0.1f,
			float _epsilon = 0.1f,
			bool _show = false) :
			entityA(_entityA),
			localSocketA(_localSocketA),
			entityB(_entityB),
			localSocketB(_localSocketB),
			initialDistance(_initialDistance),
			epsilon(_epsilon),
			show(_show)
		{
		}
	public:
		ECSEntity entityA;
		Vector3 localSocketA;
		ECSEntity entityB;
		Vector3 localSocketB;
		float epsilon;
		float initialDistance;
		bool show;
	};
}