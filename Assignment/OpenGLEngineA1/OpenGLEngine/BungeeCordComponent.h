#pragma once
#include "ECSConfig.h"
namespace Reality
{
	struct BungeeCordComponent
	{
		BungeeCordComponent(float _bungeeCordConstant = 10, float _restLength = 10, ECSEntity a = ECSEntity(), ECSEntity b = ECSEntity())
			:bungeeCordConstant(_bungeeCordConstant), restLength(_restLength), entityA(a), entityB(b) {}
		float bungeeCordConstant;
		float restLength;
		ECSEntity entityA;
		ECSEntity entityB;
	};
}
