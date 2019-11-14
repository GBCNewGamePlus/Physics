#pragma once
#include "ECSConfig.h"
#include "TriangleMath.h"
#include "TransformComponent.h"

namespace Reality
{
	struct TriangleComponent
	{
		ECSEntity entityA, entityB, entityC;
		Vector3 normal;


		TriangleComponent(ECSEntity a = ECSEntity(), ECSEntity b = ECSEntity(), ECSEntity c = ECSEntity())
		{
			entityA = a;
			entityB = b;
			entityC = c;
		}

		float PointDistanceToPlane(Vector3 point) {
			// Get the three points
			Vector3 pointA = entityA.getComponent<TransformComponent>().position;
			Vector3 pointB = entityB.getComponent<TransformComponent>().position;
			Vector3 pointC = entityC.getComponent<TransformComponent>().position;

			// Get the two vectors
			Vector3 BA = pointA - pointB;
			Vector3 BC = pointC - pointB;

			Vector3 crossResult = TriangleMath::CROSS(BA, BC);
			float constant = -crossResult.x * pointA.x - crossResult.y * pointA.y - crossResult.z * pointA.z;
			Vector4 planeEquation;
			planeEquation.x = normal.x = crossResult.x;
			planeEquation.y = normal.y = crossResult.y;
			planeEquation.z = normal.z = crossResult.z;
			planeEquation.w = constant;
			normal = TriangleMath::SDIVISION(normal);
			// based on a true story: http://mathworld.wolfram.com/Point-PlaneDistance.html
			float distance = (planeEquation.x * point.x + planeEquation.y * point.y + planeEquation.z * point.z + planeEquation.w)
				/ sqrt(planeEquation.x * planeEquation.x + planeEquation.y * planeEquation.y + planeEquation.z * planeEquation.z);
			return distance;
		}
	};
}
