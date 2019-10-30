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
				Vector3 cameraPos = getWorld().data.renderUtil->camera.Position;
				// MoveS
				// Spawns another Bungee from here
				auto subE = getWorld().createEntity();
				subE.addComponent<TransformComponent>(cameraPos);
				//subE.addComponent<ParticleComponent>();
				float testAboveHeight = cameraPos.y - 0.3f;
				subE.addComponent<BuoyancyComponent>(subE, e, testAboveHeight);
				subE.addComponent<SphereComponent>(1.0f);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && !keyReleased) {
			keyReleased = true;
		}
	}
}
