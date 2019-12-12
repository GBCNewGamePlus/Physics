#include "JointSystem.h"
#include "RigidBodyComponent.h"
#include "RigidBodySystem.h"
#include "ParticleContactComponent.h"
#include "SphereColliderComponent.h"
#include "ContactEvent.h"

namespace Reality
{
	JointSystem::JointSystem()
	{
		requireComponent<JointComponent>();
	}

	void JointSystem::Update(float deltaTime)
	{
		/*for (auto e : getEntities())
		{
			auto& cable = e.getComponent<JointComponent>();
			float length = glm::length(cable.entityA.getComponent<TransformComponentV2>().GetPosition() -
				cable.entityB.getComponent<TransformComponentV2>().GetPosition());

			if (length < cable.error)
			{
				continue;
			}

			Vector3 normal = glm::normalize(cable.entityB.getComponent<TransformComponentV2>().GetPosition() -
				cable.entityA.getComponent<TransformComponentV2>().GetPosition());

			float penetration = length - cable.error;

			auto contactEntity = getWorld().createEntity();
			contactEntity.addComponent<ParticleContactComponent>(
				cable.entityA,
				cable.entityB,
				1,
				normal,
				penetration);
		}*/

		for (auto e : getEntities())
		{
			auto &joint = e.getComponent<JointComponent>();

			if (!joint.entityA.hasComponent<TransformComponentV2>() &&
				!joint.entityB.hasComponent<TransformComponentV2>())
			{
				continue;
			}
			auto &bone1 = joint.entityA.getComponent<TransformComponentV2>();
			auto &bone2 = joint.entityB.getComponent<TransformComponentV2>();
			Vector3 b1 = bone1.LocalToWorldPosition(joint.pos1);
			Vector3 b2 = bone2.LocalToWorldPosition(joint.pos2);
			getWorld().data.renderUtil->DrawSphere(b1, 1, Color::Red);
			if (glm::length(b1-b2)<joint.error)
			{
				continue;
			}
			Vector3 normal = glm::normalize(b2 - b1);
			auto contact = getWorld().createEntity();
			
			getWorld().getEventManager().emitEvent<ContactEvent>(
				joint.entityA, joint.entityB, -normal, glm::length(b1 - b2) - joint.error,
				joint.pos1, joint.pos2, b1, b2, 0);

		}
	}
}
