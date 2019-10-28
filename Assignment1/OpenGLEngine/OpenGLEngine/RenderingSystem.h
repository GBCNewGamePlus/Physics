#pragma once
#include "ECSConfig.h"
#include "TransformComponent.h"
#include "MeshComponent.h"

class Shader;
class Camera;

namespace Reality
{
	class RenderingSystem : public ECSSystem
	{
	public:
		RenderingSystem();
		void Update(float deltaTime);
	};
}

