#include "JointSystem.h"
#include "TransformComponent.h"
#include "ParticleContactComponent.h"
#include "TransformComponentV2.h"
#include "ContactEvent.h"
#include <React3D/reactphysics3d.h>
#include <React3D/collision/ContactManifold.h>
#include <React3D/constraint/ContactPoint.h>

namespace Reality
{
	JointSystem::JointSystem()
	{
		requireComponent<JointSystem>();
	}

	void JointSystem::Update(float deltaTime)
	{
		for (auto e : getEntities())
		{
			auto& cable = e.getComponent<Joint>();
			float length = glm::length(cable.entityA.getComponent<TransformComponentV2>().GetPosition() -
				cable.entityB.getComponent<TransformComponentV2>().GetPosition());

			getWorld().data.renderUtil->DrawSphere(cable.entityA.getComponent<TransformComponentV2>().GetPosition(), 1, Color::Magenta);
			getWorld().data.renderUtil->DrawSphere(cable.entityB.getComponent<TransformComponentV2>().GetPosition(), 1, Color::Magenta);

			getWorld().data.renderUtil->DrawLine(cable.entityA.getComponent<TransformComponentV2>().GetPosition(),
				cable.entityB.getComponent<TransformComponentV2>().GetPosition(), Color::Blue);

			if (length < cable.maxLength)
			{
				continue;
			}

			Vector3 normal = glm::normalize(cable.entityB.getComponent<TransformComponentV2>().GetPosition() -
				cable.entityA.getComponent<TransformComponentV2>().GetPosition());

			float penetration = length - cable.maxLength;

			//auto contactEntity = getWorld().createEntity();
			//contactEntity.addComponent<ParticleContactComponent>(
			//	cable.entityA,
			//	cable.entityB,
			//	cable.restitution,
			//	normal,
			//	penetration);

			getWorld().getEventManager().emitEvent<ContactEvent>(cable.entityA, cable.entityB);
		}
	}
}
