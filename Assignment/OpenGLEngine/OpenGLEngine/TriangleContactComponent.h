#pragma once
#include "ECSConfig.h"

namespace Reality
{
	struct TriangleContactComponent
	{
		TriangleContactComponent(ECSEntity a = ECSEntity(),
			ECSEntity b1 = ECSEntity(), ECSEntity b2 = ECSEntity(), ECSEntity b3 = ECSEntity(),
			float _restitution = 1,
			Vector3 _normal = Vector3(0, 1, 0), float _penetration = 0) :
			entityA(a),
			entityB1(b1),
			entityB2(b2),
			entityB3(b3),
			restitution(_restitution),
			normal(_normal),
			penetration(_penetration),
			deltaMovePerMass(Vector3(0, 0, 0)) {}
		ECSEntity entityA;
		ECSEntity entityB1;
		ECSEntity entityB2;
		ECSEntity entityB3;
		float restitution;
		Vector3 normal;
		float penetration;
		Vector3 deltaMovePerMass;
	};
}
