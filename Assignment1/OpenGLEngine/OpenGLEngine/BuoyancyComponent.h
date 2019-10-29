#pragma once
#include "ECSConfig.h"
namespace Reality
{
	struct BuoyancyComponent
	{
		BuoyancyComponent(ECSEntity _sphere = ECSEntity(), ECSEntity _vat = ECSEntity(), float _liquidHeight = 0)
			: sphere(_sphere), vat(_vat), liquidHeight(_liquidHeight) {}
		ECSEntity sphere;
		ECSEntity vat;
		float liquidHeight;
	};
}
