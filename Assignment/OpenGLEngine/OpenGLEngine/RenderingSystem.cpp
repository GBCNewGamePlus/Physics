#include "RenderingSystem.h"
#include "Shader.h"
#include "Camera.h"
#include <glm/gtx/euler_angles.hpp> 

namespace Reality
{
	RenderingSystem::RenderingSystem()
	{
		requireComponent<TransformComponent>();
		//requireComponent<ModelComponent>();
	}

	void RenderingSystem::Update(float deltaTime)
	{
		if (glfwGetKey(getWorld().data.renderUtil->window->glfwWindow, GLFW_KEY_F1) == GLFW_PRESS && !drawModeChanged)
		{
			drawMode++;
			drawMode = drawMode % 2;
			drawModeChanged = true;
		}
		else if (glfwGetKey(getWorld().data.renderUtil->window->glfwWindow, GLFW_KEY_F1) == GLFW_RELEASE)
		{
			drawModeChanged = false;
		}
		getWorld().data.renderUtil->SetFOV(45);
		getWorld().data.renderUtil->UpdateViewMatrix();

		for (auto e : getEntities()) 
		{
			const auto transform = e.getComponent<TransformComponent>();
			if (e.hasComponent<ModelComponent>()) {
				auto &mesh = e.getComponent<ModelComponent>();

				if (getWorld().data.assetLoader->ModelsLoaded())
				{
					getWorld().data.assetLoader->SetLight(getWorld().data.renderUtil->camera.Position);
				}
				if (mesh.modelId < 0)
				{
					mesh.modelId = getWorld().data.assetLoader->GetModelId(mesh.mesh);
				}
				if (mesh.modelId >= 0)
				{
					getWorld().data.renderUtil->DrawModel(mesh.modelId, transform.position, transform.scale, transform.eulerAngles, drawModes[drawMode]);
				}
			}
			else if (e.hasComponent<SphereComponent>()) {
				auto &sphere = e.getComponent<SphereComponent>();
				getWorld().data.renderUtil->DrawSphere(transform.position, sphere.radius);
			}
		}
	}
}

