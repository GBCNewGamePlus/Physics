#pragma once
#include "ECSConfig.h"

namespace Reality
{
	struct TriangleComponent
	{
		ECSEntity entityA, entityB, entityC;

		TriangleComponent(ECSEntity a = ECSEntity(), ECSEntity b = ECSEntity(), ECSEntity c = ECSEntity())
		{
			entityA = a;
			entityB = b;
			entityC = c;
		}
	};
}
