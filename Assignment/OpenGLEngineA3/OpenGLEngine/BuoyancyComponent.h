#pragma once
#include "ECSConfig.h"
namespace Reality
{
	struct BuoyancyComponent
	{
		BuoyancyComponent(ECSEntity _vat = ECSEntity(), float _liquidHeight = 0, float _maxY = 0)
			: vat(_vat), liquidHeight(_liquidHeight), maxY(_maxY){}
		ECSEntity vat;
		float liquidHeight;
		float maxY;
	};
}
