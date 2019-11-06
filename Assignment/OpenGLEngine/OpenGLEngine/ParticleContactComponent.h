#pragma once
#include "ECSConfig.h"

namespace Reality
{
	struct ParticleContactComponent
	{
		ParticleContactComponent(ECSEntity a = ECSEntity(), 
			ECSEntity b = ECSEntity(), 
			float _restitution = 1, 
			Vector3 _normal = Vector3(0,1,0)):
			entityA(a),
			entityB(b),
			restitution(_restitution),
			normal(_normal){}
		ECSEntity entityA;
		ECSEntity entityB;
		float restitution;
		Vector3 normal;
	};
}