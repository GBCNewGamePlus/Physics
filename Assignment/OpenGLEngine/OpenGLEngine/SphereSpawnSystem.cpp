#include "SphereSpawnSystem.h"
#include <cstdlib>
#include <ctime>

namespace Reality
{

	SphereSpawnSystem::SphereSpawnSystem()
	{
		srand(static_cast <unsigned> (time(0)));
		UniqueSphere = NULL;
	}

	void SphereSpawnSystem::Update(float deltaTime)
	{
		GLFWwindow*  window = getWorld().data.renderUtil->window->glfwWindow;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && keyReleased)
		{
			keyReleased = false;
			if (UniqueSphere == NULL)
			{
				UniqueSphere = getWorld().createEntity();
				float randomX = ((float)rand() / RAND_MAX) * (0 - (-20)) + (-20);
				UniqueSphere.addComponent<TransformComponent>(Vector3(randomX, 20.0f, -22.5f));
				UniqueSphere.addComponent<ParticleComponent>();
				UniqueSphere.addComponent<SphereComponent>(1);
			}
			else
			{
				float randomX = ((float)rand() / RAND_MAX) * (0 - (-20)) + (-20);
				UniqueSphere.getComponent<TransformComponent>().position = Vector3(randomX, 20.0f, -22.5f);
				UniqueSphere.getComponent<ParticleComponent>().velocity = Vector3(0,0,0);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE && !keyReleased) 
		{
			keyReleased = true;
		}
	}
}

