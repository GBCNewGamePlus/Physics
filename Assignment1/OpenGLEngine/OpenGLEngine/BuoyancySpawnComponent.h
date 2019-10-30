#pragma once
namespace Reality
{
	struct BuoyancySpawnComponent
	{
		BuoyancySpawnComponent(float _liquidDensity = 1, float _densityStep = 0.1f) 
		: liquidDensity(_liquidDensity), densityStep(_densityStep){}
		float liquidDensity;
		float densityStep;
	};
}
