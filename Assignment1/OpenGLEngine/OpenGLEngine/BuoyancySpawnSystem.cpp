#include "BuoyancySpawnSystem.h"

namespace Reality
{
	BuoyancySpawnSystem::BuoyancySpawnSystem()
	{
		requireComponent<BuoyancySpawnComponent>();
	}
	void BuoyancySpawnSystem::Update(float deltaTime)
	{
		GLFWwindow*  window = getWorld().data.renderUtil->window->glfwWindow;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && keyReleased)
		{
			keyReleased = false;
			for (auto e : getEntities())
			{
				// MoveS
				// Spawns another Bungee from here
				auto subE = getWorld().createEntity();
				//float rx = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 4 - 2;
				//float ry = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 4 - 2;
				//float rz = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 4 - 2;
				//subE.addComponent<TransformComponent>(transformComponent.position + Vector3(rx, ry, rz));
				subE.addComponent<TransformComponent>(getWorld().data.renderUtil->camera.Position);
				subE.addComponent<ParticleComponent>();
				//subE.addComponent<BuoyancyComponent>(10, 2.5f, e, subE);
				subE.addComponent<SphereComponent>(1);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && !keyReleased) {
			keyReleased = true;
		}
	}
}
