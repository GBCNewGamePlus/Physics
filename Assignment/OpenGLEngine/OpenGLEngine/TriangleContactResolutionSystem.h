#pragma once
#include "ECSConfig.h"
#include "TriangleContactComponent.h"

namespace Reality
{
	class TriangleContactResolutionSystem : public ECSSystem
	{
	public:
		TriangleContactResolutionSystem();
		void Update(float deltaTime);
		unsigned int iterations = 1;
	private:
		float CalculateSeparatingVelocity(TriangleContactComponent& contact);
		void ResolveVelocity(TriangleContactComponent& contact, float deltaTime);
		void ResolveInterpenetration(TriangleContactComponent& contact);
		void UpdateInterpenetration(TriangleContactComponent& bestContact, TriangleContactComponent& contact);
		unsigned int iterationsUsed = 0;
	};
}
