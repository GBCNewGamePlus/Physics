#include "JointSystem.h"
#include "JointComponent.h"
#include "ECSConfig.h"
#include "TransformComponentV2.h"
#include "ContactEvent.h"
#include "RigidBodyComponent.h"
#include <React3D/reactphysics3d.h>
#include <React3D/collision/ContactManifold.h>
#include <React3D/constraint/ContactPoint.h>

namespace Reality
{
	JointSystem::JointSystem()
	{
		requireComponent<JointComponent>();
	}

	void JointSystem::Update(float deltaTime)
	{
		for (auto e : getEntities())
		{
			auto &joint = e.getComponent<JointComponent>();
			auto &transformA = joint.entityA.getComponent<TransformComponentV2>();
			auto &transformB = joint.entityB.getComponent<TransformComponentV2>();
			//
			Vector3 socketAPosition = 
				transformA.LocalToWorldPosition(joint.localSocketA);
			Vector3 socketBPosition = 
				transformB.LocalToWorldPosition(joint.localSocketB);
			float distance = glm::length(socketAPosition - socketBPosition);
			Vector3 normal = glm::normalize(socketAPosition - socketBPosition);
			if (joint.show)
			{
				getWorld().data.renderUtil->DrawSphere(socketAPosition, 0.1, Color::Red);
				getWorld().data.renderUtil->DrawSphere(socketBPosition, 0.1, Color::Blue);
			}

			if (distance > joint.epsilon) {
				auto contact = getWorld().createEntity();
				getWorld().getEventManager().emitEvent<ContactEvent>
					(joint.entityA, 
					 joint.entityB,
					 normal,
					 distance,
					 joint.localSocketA,
					 joint.localSocketB,
					 socketAPosition, 
					 socketBPosition, 0);
			}
			//else if (distance < joint.initialDistance + joint.epsilon) {
			//	//auto contact = getWorld().createEntity();
			//	getWorld().getEventManager().emitEvent<ContactEvent>
			//		(joint.entityA,
			//		 joint.entityB,
			//		 normal,
			//		 abs(distance - joint.initialDistance),
			//		 joint.localSocketA,
			//		 joint.localSocketB,
			//		 socketAPosition,
			//		 socketBPosition, 0);
			//}
			//else, who cares?
		}
	}
}
