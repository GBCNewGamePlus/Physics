#include "BuoyancyForceGeneratorSystem.h"
#define PI 3.141592f

namespace Reality
{
	BuoyancyForceGeneratorSystem::BuoyancyForceGeneratorSystem()
	{
		requireComponent<BuoyancyComponent>();
		requireComponent<TransformComponentV2>();
		requireComponent<RigidBodyComponent>();
		localZ[0] = localZ[1] = localZ[2] = localZ[3] = 0.001;
		localX[0] = localX[1] = localX[2] = localX[3] = 0.001;
	}


	void BuoyancyForceGeneratorSystem::Update(float deltaTime)
	{
		for (auto e : getEntities())
		{
			auto &buoyancy = e.getComponent<BuoyancyComponent>();
			auto &rigidBody = e.getComponent<RigidBodyComponent>();
			auto &transform = e.getComponent<TransformComponentV2>();

			float immersedHeight = buoyancy.liquidHeight - (transform.GetPosition().y - transform.GetScale().y / 2);
			float immersedVolume = transform.GetScale().x * transform.GetScale().z * immersedHeight;
			
			Vector3 rbc = transform.LocalToWorldPosition(buoyancy.centerOfBuoyancy);
			Vector3 finalForce = immersedVolume * buoyancy.liquidDensity * Vector3(0, 1, 0);
			rigidBody.AddForceAtPoint(finalForce * rigidBody.gravityScale / rigidBody.inverseMass,
				Vector3(rbc.x,rbc.y,rbc.z), transform.GetPosition());

			// Calculate the next one
			float cRX = transform.GetRotation().x;
			float lbcz = -(0.001f) * cRX / (PI / 3);
			localZ[pointer % 4] = lbcz;

			float cRZ = transform.GetRotation().z;
			float lbcx = -(0.001f) * cRZ/ (PI / 3);
			localX[pointer % 4] = lbcx;


			pointer++;
			buoyancy.centerOfBuoyancy = Vector4(0, localX[pointer % 4], localZ[pointer % 4], 0);

		}
	}
}