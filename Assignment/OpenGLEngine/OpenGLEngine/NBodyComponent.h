#pragma once
#include "ECSConfig.h"
namespace Reality
{
	struct NBodyComponent
	{
		NBodyComponent(ECSEntity a = ECSEntity(), ECSEntity b = ECSEntity(), ECSEntity c = ECSEntity(), ECSEntity d = ECSEntity(), ECSEntity e = ECSEntity(), ECSEntity f = ECSEntity(), ECSEntity g = ECSEntity(), ECSEntity h = ECSEntity(), ECSEntity i = ECSEntity(), ECSEntity j = ECSEntity())
			: entity0(a), entity1(b), entity2(c), entity3(d), entity4(e), entity5(f), entity6(g), entity7(h), entity8(i), entity9(j) {}
		ECSEntity entity0;
		ECSEntity entity1;
		ECSEntity entity2;
		ECSEntity entity3;
		ECSEntity entity4;
		ECSEntity entity5;
		ECSEntity entity6;
		ECSEntity entity7;
		ECSEntity entity8;
		ECSEntity entity9;
	};
}
