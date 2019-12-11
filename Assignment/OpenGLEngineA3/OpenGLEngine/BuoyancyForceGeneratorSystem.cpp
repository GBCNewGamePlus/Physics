#include "BuoyancyForceGeneratorSystem.h"
#define PI 3.141592f

namespace Reality
{
	BuoyancyForceGeneratorSystem::BuoyancyForceGeneratorSystem()
	{
		requireComponent<BuoyancyComponent>();
		requireComponent<TransformComponent>();
		requireComponent<ParticleComponent>();
		requireComponent<SphereComponent>();
	}


	void BuoyancyForceGeneratorSystem::Update(float deltaTime)
	{
		for (auto e : getEntities())
		{
			auto &buoyancy = e.getComponent<BuoyancyComponent>();
			auto &particle = e.getComponent<ParticleComponent>();
			auto &sphere = e.getComponent<SphereComponent>();
			auto &sphereTransform = e.getComponent<TransformComponent>();
			auto &vatTransform = buoyancy.vat.getComponent<TransformComponent>();
			auto &buoyancySpawn = buoyancy.vat.getComponent<BuoyancySpawnComponent>();

			Vector3 minSphere = sphereTransform.position - 0.5f*sphereTransform.scale;
			Vector3 maxSphere = sphereTransform.position + 0.5f*sphereTransform.scale;

			Vector3 minVat = vatTransform.position - 0.5f*vatTransform.scale;
			Vector3 maxVat = vatTransform.position + 0.5f*vatTransform.scale;

			// Verifies "immersion" between vat and sphere
			if (minSphere.x > minVat.x && maxSphere.x < maxVat.x
				&& minSphere.z > minVat.z && maxSphere.z < maxVat.z) {
				// Now it is just a matter of verifying the y
				buoyancy.liquidHeight = maxVat.y;

				std::string VatHeight = std::to_string(maxVat.y);
				std::string SphereHeight = std::to_string(minSphere.y);

				getWorld().data.renderUtil->RenderText("Current sphere: " + SphereHeight + " current vat: " + VatHeight, 1620.0f, 980.0f, 0.25f, Color(0, 1, 1, 1));

				// hasn't touched the liquid yet
				if (minSphere.y >= maxVat.y)
					return;
				float immersedVolume = 0.0;
				if (minSphere.y < maxVat.y && maxSphere.y > maxVat.y) {
					// Partially immersed from the Top
					float h = maxSphere.y - maxVat.y;
					immersedVolume = (PI * pow(h, 2.0f) * (3 * sphere.radius - h)) / 3.0f;
				}
				else if (maxSphere.y < maxVat.y && minSphere.y > minVat.y)
				{
					// Fully immersed
					immersedVolume = (4.0f * PI * pow(sphere.radius,3.0f))/3.0f;
				}
				else if (minSphere.y < minVat.y && maxSphere.y > minVat.y) {
					// Partially immersed from the Bottom
					float h = maxSphere.y - minVat.y;
					immersedVolume = (PI * pow(h, 2.0f) * (3 * sphere.radius - h)) / 3.0f;
				}
				particle.AddForce(immersedVolume * buoyancySpawn.liquidDensity * Vector3(0, 1, 0));
			}
		}
	}
}