#pragma once
#include "ECSConfig.h"
#include "TriangleMath.h"
#include "TransformComponent.h"

namespace Reality
{
	struct TriangleComponent
	{
		ECSEntity entityA, entityB, entityC;
		Vector3 AB, BC, CA;
		Vector3 normalAB, normalBC, normalCA;
		Vector3 normalPlane;
		float direction;
		Vector3 tempa;
		Vector3 tempb;
		Vector3 tempc;

		TriangleComponent(ECSEntity a = ECSEntity(), ECSEntity b = ECSEntity(), ECSEntity c = ECSEntity(), Vector3 _a = Vector3(0,0,0), Vector3 _b = Vector3(0, 0, 0), Vector3 _c = Vector3(0, 0, 0))
		{
			entityA = a;
			entityB = b;
			entityC = c;
			tempa = _a;
			tempb = _b;
			tempc = _c;
			AB = tempa - tempb;
			BC = tempb - tempc;
			CA = tempc - tempa;
			normalAB = TriangleMath::SDIVISION(AB);
			normalBC = TriangleMath::SDIVISION(BC);
			normalCA = TriangleMath::SDIVISION(CA);
			normalPlane = TriangleMath::CROSS(normalAB,normalBC);
			direction = TriangleMath::DOT(normalPlane, tempa);
		}
	};
}
