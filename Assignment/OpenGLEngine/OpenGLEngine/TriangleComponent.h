#pragma once
#include "ECSConfig.h"
#include "TriangleMath.h"
#include "TransformComponent.h"

namespace Reality
{
	struct TriangleComponent
	{
		ECSEntity entityA, entityB, entityC;
		Vector4 planeEquation;
		Vector3 normal;

		TriangleComponent(ECSEntity a = ECSEntity(), ECSEntity b = ECSEntity(), ECSEntity c = ECSEntity())
		{
			entityA = a;
			entityB = b;
			entityC = c;
			if (a.getIndex()
				&& b.getIndex()
				&& c.getIndex())
			{
				// Get the three points
				Vector3 pointA = a.getComponent<TransformComponent>().position;
				Vector3 pointB = b.getComponent<TransformComponent>().position;
				Vector3 pointC = c.getComponent<TransformComponent>().position;

				// Get the two vectors
				Vector3 BA = pointA - pointB;
				Vector3 BC = pointC - pointB;

				Vector3 crossResult = TriangleMath::CROSS(BA, BC);
				float constant = -crossResult.x * pointA.x - crossResult.y * pointA.y - crossResult.z * pointA.z;

				planeEquation.x = normal.x = crossResult.x;
				planeEquation.y = normal.y = crossResult.y;
				planeEquation.z = normal.z = crossResult.z;
				planeEquation.w = constant;
				normal = TriangleMath::SDIVISION(normal);
			}
		}

		float PointDistanceToPlane(Vector3 point) {
			// based on a true story: http://mathworld.wolfram.com/Point-PlaneDistance.html

			return (planeEquation.x * point.x + planeEquation.y * point.y + planeEquation.z * point.z + planeEquation.w)
				   / sqrt(planeEquation.x * planeEquation.x + planeEquation.y * planeEquation.y + planeEquation.z * planeEquation.z);
		}
	};
}
