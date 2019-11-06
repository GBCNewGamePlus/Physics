#include "ParticleContactResolutionSystem.h"
#include "ParticleComponent.h"
#include "ParticleContactEvent.h"

namespace Reality
{
	ParticleContactResolutionSystem::ParticleContactResolutionSystem()
	{
		requireComponent<ParticleContactEvent>();
	}

	float ParticleContactResolutionSystem::CalculateSeparatingVelocity(ParticleContactEvent& contact)
	{
		Vector3 velocityA = contact.entityA.getComponent<ParticleComponent>().velocity;
		Vector3 velocityB = contact.entityB.getComponent<ParticleComponent>().velocity;
		Vector3 relativeVel = velocityA - velocityB;
		return glm::dot(relativeVel, contact.normal);
	}
	
	void ParticleContactResolutionSystem::ResolveVelocity(ParticleContactEvent& contact)
	{
		float separatingVelocity = CalculateSeparatingVelocity(contact);

		if (separatingVelocity > 0)
		{
			return;
		}

		auto &particleA = contact.entityA.getComponent<ParticleComponent>();
		auto &particleB = contact.entityB.getComponent<ParticleComponent>();


		float newSeparatingVelocity = -separatingVelocity * contact.restitution;
		float deltaVelocity = newSeparatingVelocity - separatingVelocity;

		float totalInverseMass = particleA.inverseMass + particleB.inverseMass;

		if (totalInverseMass <= 0)
		{
			return;
		}

		float impulse = deltaVelocity / totalInverseMass;

		Vector3 impulsePerIMass = contact.normal * impulse;

		particleA.velocity += impulsePerIMass * particleA.inverseMass;
		particleB.velocity += impulsePerIMass * particleB.inverseMass;
	}

	void ParticleContactResolutionSystem::Update(float deltaTime)
	{
		auto contactEvents = getWorld().getEventManager().getEvents<ParticleContactEvent>();
		for (auto contact : contactEvents)
		{
			ResolveVelocity(contact);
		}

		/*
		for (auto e : getEntities())
		{
			auto &contact = e.getComponent<ParticleContactComponent>();
			ResolveVelocity(contact);
			e.kill();
		}
		*/
	}
}
