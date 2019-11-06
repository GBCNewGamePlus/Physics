#include "NBodyForceGeneratorSystem.h"

namespace Reality
{
	NBodyForceGeneratorSystem::NBodyForceGeneratorSystem()
	{
		requireComponent<NBodyComponent>();
	}


	void NBodyForceGeneratorSystem::Update(float deltaTime)
	{
		for (auto e : getEntities())
		{
			auto &nBody = e.getComponent<NBodyComponent>();
			ECSEntity* objHolder[10];
			objHolder[0] = &nBody.entity0;
			objHolder[1] = &nBody.entity1;
			objHolder[2] = &nBody.entity2;
			objHolder[3] = &nBody.entity3;
			objHolder[4] = &nBody.entity4;
			objHolder[5] = &nBody.entity5;
			objHolder[6] = &nBody.entity6;
			objHolder[7] = &nBody.entity7;
			objHolder[8] = &nBody.entity8;
			objHolder[9] = &nBody.entity9;
			bool safetycheck = true;
			int size = 0;
			for (ECSEntity* x :  objHolder) {
				size++;
				if (!x->hasComponent<TransformComponent>())
					if (!x->hasComponent<ParticleComponent>())
						safetycheck = false;
			}
			if (safetycheck)
			{
				for (int i = 0; i < size; i++) {
					auto &entityTransformA = objHolder[i]->getComponent<TransformComponent>();
					auto &entityMassA = objHolder[i]->getComponent<ParticleComponent>();
					for (int j = 0; j < size; j++) {
						if (j != i) {
							auto &entityTransformB = objHolder[j]->getComponent<TransformComponent>();
							auto &entityMassB = objHolder[j]->getComponent<ParticleComponent>();

							Vector3 relativePosition = entityTransformA.position - entityTransformB.position;
							float length = glm::length(relativePosition);

							Vector3 direction = glm::normalize(relativePosition);

							float force = 1 * ((entityMassA.Mass - entityMassB.Mass) / length);

							objHolder[i]->getComponent<ParticleComponent>().AddForce(force * direction);
						}
					}

				}
			}
		}
	}
}