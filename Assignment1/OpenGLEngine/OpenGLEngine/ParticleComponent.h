#pragma once
#include "ECSConfig.h"

namespace Reality
{
	struct ParticleComponent
	{
		ParticleComponent(float mass = 1.0f, Vector3 _velocity = Vector3(0,0,0)) :
			velocity(_velocity)
		{
			inverseMass = 1 / mass;
			Mass = mass;
			accelaration = Vector3(0, 0, 0);
			forceAccumulator = Vector3(0, 0, 0);
		}
		Vector3 velocity;
		Vector3 accelaration;
		float inverseMass;
		float Mass;
		inline void AddForce(Vector3 force)
		{
			forceAccumulator += force;
		}
		inline Vector3 GetForce()
		{
			return forceAccumulator;
		}
		inline void ResetForceAccumulator()
		{
			forceAccumulator = Vector3(0, 0, 0);
		}
	private:
		Vector3 forceAccumulator;
	};
}
