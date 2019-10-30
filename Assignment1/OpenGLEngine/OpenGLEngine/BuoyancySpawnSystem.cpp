#include "BuoyancySpawnSystem.h"

namespace Reality
{
	BuoyancySpawnSystem::BuoyancySpawnSystem()
	{
		requireComponent<BuoyancySpawnComponent>();
		requireComponent<TransformComponent>();
	}
	void BuoyancySpawnSystem::Update(float deltaTime)
	{
		GLFWwindow*  window = getWorld().data.renderUtil->window->glfwWindow;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && keyReleased)
		{
			keyReleased = false;
			for (auto e : getEntities())
			{
				auto &transform = e.getComponent<TransformComponent>();
				auto &buoyancySpawn = e.getComponent<BuoyancySpawnComponent>();
				/* Considering origin the middle point */
				float maxY = transform.position.y - transform.scale.y;
				Vector3 cameraPos = getWorld().data.renderUtil->camera.Position;
				// MoveS
				// Spawns another Bungee from here
				auto subE = getWorld().createEntity();
				subE.addComponent<TransformComponent>(cameraPos);
				//subE.addComponent<TransformComponent>(transform.position + Vector3(0,8,0));
				subE.addComponent<ParticleComponent>();
				//float testAboveHeight = cameraPos.y - 0.3f;
				subE.addComponent<BuoyancyComponent>(e, 1.5 * maxY, maxY);
				subE.addComponent<SphereComponent>(1.0f);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && !keyReleased) {
			keyReleased = true;
		}
	}
}
