#include "BuoyancyDensitySystem.h"

namespace Reality
{
	BuoyancyDensitySystem::BuoyancyDensitySystem()
	{
		requireComponent<BuoyancySpawnComponent>();
	}
	void BuoyancyDensitySystem::Update(float deltaTime)
	{
		GLFWwindow*  window = getWorld().data.renderUtil->window->glfwWindow;
		float finalDensity = 0;

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && keyReleased)
		{
			keyUsed = GLFW_KEY_UP;
			keyReleased = false;
			for (auto e : getEntities())
			{
				auto &buoyancy = e.getComponent<BuoyancySpawnComponent>();
				buoyancy.liquidDensity += buoyancy.densityStep;
				finalDensity = buoyancy.liquidDensity;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && keyReleased)
		{
			keyUsed = GLFW_KEY_DOWN;
			keyReleased = false;
			for (auto e : getEntities())
			{
				auto &buoyancy = e.getComponent<BuoyancySpawnComponent>();
				if (buoyancy.liquidDensity > buoyancy.densityStep)
				{
					buoyancy.liquidDensity -= buoyancy.densityStep;
				}
			}
		}
		if (keyUsed == GLFW_KEY_UP && glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE && !keyReleased) {
			keyReleased = true;
		}
		if (keyUsed == GLFW_KEY_DOWN && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE && !keyReleased) {
			keyReleased = true;
		}
		for (auto e : getEntities())
		{
			auto &buoyancy = e.getComponent<BuoyancySpawnComponent>();
			finalDensity = buoyancy.liquidDensity;
			break;
		}
		std::string densityString = std::to_string(finalDensity);
		getWorld().data.renderUtil->RenderText("Current Liquid Density is " + densityString, 1540.0f, 1000.0f, 0.25f, Color(0, 1, 1, 1));
	}
}
