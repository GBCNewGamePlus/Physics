#pragma once
#include "ECSConfig.h"

namespace Reality
{
	struct TriangleComponent
	{
		ECSEntity entityA, entityB, entityC;
		Vector3 AB, BC, CA;
		Vector3 normalAB, normalBC, normalCA;
		Vector3 normalPlane;

		TriangleComponent(ECSEntity a = ECSEntity(), ECSEntity b = ECSEntity(), ECSEntity c = ECSEntity())
		{
			entityA = a;
			entityB = b;
			entityC = c;
			auto& tempa = a.getComponent<TransformComponent>();
			auto& tempb = b.getComponent<TransformComponent>();
			auto& tempc = c.getComponent<TransformComponent>();
			AB = tempa.position - tempb.position;
			BC = tempb.position - tempc.position;
			CA = tempc.position - tempa.position;
			normalAB = AB / AB.length;
			normalBC = BC / BC.length;
			normalCA = CA / CA.length;
			normalPlane = Vector3((normalAB.y*normalBC.z) - (normalAB.z*normalBC.y), (normalAB.z*normalBC.x) - (normalAB.x*normalBC.z), (normalAB.x*normalBC.y) - (normalAB.y*normalBC.x));
		}
	};
}
