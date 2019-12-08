#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "RenderUtil.h"
#include <memory>
class WorldData
{
public:
	std::unique_ptr<Reality::RenderUtil> renderUtil;

	void InitRendering()
	{
		renderUtil = std::make_unique< Reality::RenderUtil>();
	}
};
