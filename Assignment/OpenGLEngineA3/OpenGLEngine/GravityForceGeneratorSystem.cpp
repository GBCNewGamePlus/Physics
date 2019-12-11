#include "GravityForceGeneratorSystem.h"


namespace Reality
{
	GravityForceGeneratorSystem::GravityForceGeneratorSystem()
	{
		requireComponent<RigidBodyComponent>();
	}


	void GravityForceGeneratorSystem::Update(float deltaTime)
	{
		for (auto e : getEntities())
		{
			auto &rigidBody = e.getComponent<RigidBodyComponent>();
			rigidBody.AddForce(gravity * rigidBody.gravityScale / rigidBody.inverseMass);
		}

	}
}
