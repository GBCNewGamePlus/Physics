#include "TriangleContactResolutionSystem.h"
#include "ParticleComponent.h"
#include "TransformComponent.h"

namespace Reality
{
	TriangleContactResolutionSystem::TriangleContactResolutionSystem()
	{
		requireComponent<TriangleContactComponent>();

	}


	float TriangleContactResolutionSystem::CalculateSeparatingVelocity(TriangleContactComponent& contact)
	{
		Vector3 velocityA = contact.entityA.hasComponent<ParticleComponent>() ? 
			contact.entityA.getComponent<ParticleComponent>().velocity : Vector3(0, 0, 0);
		Vector3 velocityB1 = contact.entityB1.hasComponent<ParticleComponent>() ?
			contact.entityB1.getComponent<ParticleComponent>().velocity : Vector3(0, 0, 0);
		Vector3 velocityB2 = contact.entityB1.hasComponent<ParticleComponent>() ?
			contact.entityB2.getComponent<ParticleComponent>().velocity : Vector3(0, 0, 0);
		Vector3 velocityB3 = contact.entityB1.hasComponent<ParticleComponent>() ?
			contact.entityB3.getComponent<ParticleComponent>().velocity : Vector3(0, 0, 0);
		Vector3 relativeVel = velocityA - 0.33f * (velocityB1 + velocityB2 + velocityB3);
		return glm::dot(relativeVel, contact.normal);
	}

	void TriangleContactResolutionSystem::ResolveVelocity(TriangleContactComponent& contact, float deltaTime)
	{
		float separatingVelocity = CalculateSeparatingVelocity(contact);

		if (separatingVelocity > 0)
		{
			return;
		}

		bool isAvalid = contact.entityA.hasComponent<ParticleComponent>();
		bool isB1valid = contact.entityB1.hasComponent<ParticleComponent>();
		bool isB2valid = contact.entityB2.hasComponent<ParticleComponent>();
		bool isB3valid = contact.entityB3.hasComponent<ParticleComponent>();
		float invM1 = isAvalid ? contact.entityA.getComponent<ParticleComponent>().inverseMass : 0;
		float invM21 = isB1valid ? contact.entityB1.getComponent<ParticleComponent>().inverseMass : 0;
		float invM22 = isB2valid ? contact.entityB2.getComponent<ParticleComponent>().inverseMass : 0;
		float invM23 = isB3valid ? contact.entityB3.getComponent<ParticleComponent>().inverseMass : 0;


		float newSeparatingVelocity = -separatingVelocity * contact.restitution;

		// Check the velocity build up due to accelaration only
		Vector3 accCausedVelocity = Vector3(0, 0, 0);
		if (isAvalid)
		{
			accCausedVelocity += contact.entityA.getComponent<ParticleComponent>().accelaration;
		}
		if (isB1valid)
		{
			accCausedVelocity -= contact.entityB1.getComponent<ParticleComponent>().accelaration;
		}
		if (isB2valid)
		{
			accCausedVelocity -= contact.entityB2.getComponent<ParticleComponent>().accelaration;
		}
		if (isB3valid)
		{
			accCausedVelocity -= contact.entityB3.getComponent<ParticleComponent>().accelaration;
		}
		float accCausedSepVelocity = glm::dot(accCausedVelocity, contact.normal) * deltaTime;

		// If we have a closing velocity due to accelaration build up, 
		// remove it from new separating velocity
		if (accCausedSepVelocity < 0)
		{
			newSeparatingVelocity += contact.restitution * accCausedSepVelocity;
			if (newSeparatingVelocity < 0)
			{
				newSeparatingVelocity = 0;
			}
		}

		float deltaVelocity = newSeparatingVelocity - separatingVelocity;

		float totalInverseMass = invM1 + invM21 + invM22 + invM23;

		if (totalInverseMass <= 0)
		{
			return;
		}

		float impulse = deltaVelocity / totalInverseMass;

		Vector3 impulsePerIMass = contact.normal * impulse;

		if (isAvalid)
		{
			contact.entityA.getComponent<ParticleComponent>().velocity += impulsePerIMass * invM1;
		}
		if (isB1valid)
		{
			contact.entityB1.getComponent<ParticleComponent>().velocity -= impulsePerIMass * invM21;
		}
		if (isB2valid)
		{
			contact.entityB2.getComponent<ParticleComponent>().velocity -= impulsePerIMass * invM22;
		}
		if (isB3valid)
		{
			contact.entityB3.getComponent<ParticleComponent>().velocity -= impulsePerIMass * invM23;
		}
	}

	void TriangleContactResolutionSystem::ResolveInterpenetration(TriangleContactComponent& contact)
	{
		if (contact.penetration <= 0)
		{
			return;
		}

		bool isAvalid = contact.entityA.hasComponent<ParticleComponent>();
		bool isB1valid = contact.entityB1.hasComponent<ParticleComponent>();
		bool isB2valid = contact.entityB2.hasComponent<ParticleComponent>();
		bool isB3valid = contact.entityB3.hasComponent<ParticleComponent>();
		float invM1 = isAvalid ? contact.entityA.getComponent<ParticleComponent>().inverseMass : 0;
		float invM21 = isB1valid ? contact.entityB1.getComponent<ParticleComponent>().inverseMass : 0;
		float invM22 = isB2valid ? contact.entityB2.getComponent<ParticleComponent>().inverseMass : 0;
		float invM23 = isB3valid ? contact.entityB3.getComponent<ParticleComponent>().inverseMass : 0;

		float totalInverseMass = invM1 + invM21 + invM22 + invM23;

		if (totalInverseMass <= 0)
		{
			return;
		}

		Vector3 movePerMass = contact.normal * (-contact.penetration / totalInverseMass);
		contact.deltaMovePerMass = movePerMass;
		if (isAvalid)
		{
			contact.entityA.getComponent<TransformComponent>().position -= movePerMass * invM1;
		}

		if (isB1valid)
		{
			contact.entityB1.getComponent<TransformComponent>().position += movePerMass * invM21;
		}
		if (isB2valid)
		{
			contact.entityB2.getComponent<TransformComponent>().position += movePerMass * invM22;
		}
		if (isB3valid)
		{
			contact.entityB3.getComponent<TransformComponent>().position += movePerMass * invM23;
		}
		//contact.penetration = 0;
	}

	void TriangleContactResolutionSystem::UpdateInterpenetration(TriangleContactComponent & bestContact, TriangleContactComponent & contact)
	{
		bool isAvalid = contact.entityA.hasComponent<ParticleComponent>();
		bool isB1valid = contact.entityB1.hasComponent<ParticleComponent>();
		bool isB2valid = contact.entityB2.hasComponent<ParticleComponent>();
		bool isB3valid = contact.entityB3.hasComponent<ParticleComponent>();
		float invM1 = isAvalid ? contact.entityA.getComponent<ParticleComponent>().inverseMass : 0;
		float invM21 = isB1valid ? contact.entityB1.getComponent<ParticleComponent>().inverseMass : 0;
		float invM22 = isB2valid ? contact.entityB2.getComponent<ParticleComponent>().inverseMass : 0;
		float invM23 = isB3valid ? contact.entityB3.getComponent<ParticleComponent>().inverseMass : 0;

		if (bestContact.entityA == contact.entityA || bestContact.entityB1 == contact.entityA)
		{
			float mult = bestContact.entityA == contact.entityA ? -1 : 1;
			Vector3 deltaMove = mult * bestContact.deltaMovePerMass * invM1;
			float deltaPenetration = glm::dot(deltaMove, contact.normal);
			contact.penetration -= deltaPenetration;
		}
		if (bestContact.entityB1 == contact.entityB1 || bestContact.entityA == contact.entityB1)
		{
			float mult = bestContact.entityA == contact.entityB1 ? -1 : 1;
			Vector3 deltaMove = mult * bestContact.deltaMovePerMass * invM21;
			float deltaPenetration = glm::dot(deltaMove, contact.normal);
			contact.penetration += deltaPenetration;
		}
	}
	void TriangleContactResolutionSystem::Update(float deltaTime)
	{
		iterationsUsed = 0;
		iterations = getEntities().size() * 2;

		if (getEntities().size() > 0)
		{
			unsigned int bestContactIndex = 0;
			unsigned int lastBest = 0;
			while (iterationsUsed < iterations)
			{
				// Find the contact with the largest closing velocity
				float max = 0;
				for (int i = 0; i < getEntities().size(); i++)
				{
					auto e = getEntities()[i];
					auto &contact = e.getComponent<TriangleContactComponent>();
					if (iterationsUsed > 0)
					{
						UpdateInterpenetration(getEntities()[lastBest].getComponent<TriangleContactComponent>(), contact);
					}
					float sepVel = CalculateSeparatingVelocity(contact);
					if (sepVel < max)
					{
						max = sepVel;
						bestContactIndex = i;
					}
				}
				if (max >= 0)
				{
					break;
				}
				auto& bestContact = getEntities()[bestContactIndex].getComponent<TriangleContactComponent>();
				ResolveVelocity(bestContact, deltaTime);
				ResolveInterpenetration(bestContact);
				lastBest = bestContactIndex;
				iterationsUsed++;
			}

			for (auto e : getEntities())
			{
				e.kill();
			}
		}
	}
}
