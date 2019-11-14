#pragma once
#include "ECSConfig.h"
#include "TriangleMath.h"
#include "TransformComponent.h"

namespace Reality
{
	struct TriangleComponent
	{
		ECSEntity entityA, entityB, entityC;
		Vector3 normalAB, normalBC, normalCA;
		Vector3 normalPlane;
		float direction;

		TriangleComponent(ECSEntity a = ECSEntity(), ECSEntity b = ECSEntity(), ECSEntity c = ECSEntity())
		{
			entityA = a;
			entityB = b;
			entityC = c;
			// Get the three points
			Vector3 pointA = a.getComponent<TransformComponent>().position;
			Vector3 pointB = b.getComponent<TransformComponent>().position;
			Vector3 pointC = c.getComponent<TransformComponent>().position;

			// Get the two vectors
			Vector3 BA = pointA - pointB;
			Vector3 BC = pointC - pointB;
			Vector3 Cross = TriangleMath::CROSS(BA, BC);

			
		}
	};
}
