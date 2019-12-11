#include "CableComponentSystem.h"
#include "TransformComponent.h"
#include "ParticleContactComponent.h"
#include "TransformComponentV2.h"
#include "ContactEvent.h"
#include "Joint.h"
#include <React3D/reactphysics3d.h>
#include <React3D/collision/ContactManifold.h>
#include <React3D/constraint/ContactPoint.h>

namespace Reality
{
	CableComponentSystem::CableComponentSystem()
	{
		requireComponent<CableComponent>();
	}

	void CableComponentSystem::Update(float deltaTime)
	{
		for (auto e : getEntities())
		{
			auto& cable = e.getComponent<CableComponent>();
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
			Vector3 eALocalBone = Vector3(0, 0, 0);
			Vector3 eAWorldBone = Vector3(0, 0, 0);
			Vector3 eBLocalBone = Vector3(0, 0, 0);
			Vector3 eBWorldBone = Vector3(0, 0, 0);
			for (int i = 0; i < cable.entityA.getComponent<Joint>().numJoints; i++) {
				Vector3 worldTemp = cable.entityA.getComponent<TransformComponentV2>().LocalToWorldPosition(cable.entityA.getComponent<Joint>().joint[i]);
				if (glm::length(worldTemp - cable.entityB.getComponent<TransformComponentV2>().GetPosition()) < length)

					eAWorldBone = worldTemp;
					eALocalBone = cable.entityA.getComponent<TransformComponentV2>().WorldToLocalPosition(worldTemp);
			}
			for (int i = 0; i < cable.entityB.getComponent<Joint>().numJoints; i++) {
				Vector3 worldTemp = cable.entityB.getComponent<TransformComponentV2>().LocalToWorldPosition(cable.entityB.getComponent<Joint>().joint[i]);
				if (glm::length(worldTemp - cable.entityA.getComponent<TransformComponentV2>().GetPosition()) < length)

					eBWorldBone = worldTemp;
				eBLocalBone = cable.entityB.getComponent<TransformComponentV2>().WorldToLocalPosition(worldTemp);
			}
			
			Vector3 normal = (glm::normalize(cable.entityB.getComponent<TransformComponentV2>().GetPosition() -
				cable.entityA.getComponent<TransformComponentV2>().GetPosition()))*-1.0f;

			float penetration = length - cable.maxLength;

			/*auto contactEntity = getWorld().getEventManager().emitEvent<ContactEvent>(
				cable.entityA,
				cable.entityB,
				cable.restitution,
				normal,
				penetration);*/
			getWorld().getEventManager().emitEvent<ContactEvent>(cable.entityA, cable.entityB,
				normal,
				penetration,
				eALocalBone,
				eBLocalBone,
				eAWorldBone,
				eBWorldBone);
			//getWorld().getEventManager().emitEvent<ContactEvent>(cable.entityA, cable.entityB);
		}
	}
}
