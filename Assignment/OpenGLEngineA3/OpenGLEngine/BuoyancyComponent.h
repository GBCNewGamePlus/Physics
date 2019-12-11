#pragma once
#include "ECSConfig.h"
namespace Reality
{
	struct BuoyancyComponent
	{
		BuoyancyComponent(Vector4 _centerOfBuoyancy = Vector4(0,0,0,0), float _liquidHeight = 0, float _maxY = 0, 
			float _liquidDensity = 0.2, float _densityStep = 0.01f)
			: centerOfBuoyancy(_centerOfBuoyancy), liquidHeight(_liquidHeight), maxY(_maxY),
			  liquidDensity(_liquidDensity), densityStep(_densityStep){}
		Vector4 centerOfBuoyancy;
		float liquidHeight;
		float maxY;
		float liquidDensity;
		float densityStep;

	};
}
