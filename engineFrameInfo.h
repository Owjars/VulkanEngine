#pragma once

#include "engineCamera.h"
#include "engineGameObject.h"

#include <vulkan/vulkan.h>

namespace gameEngine
{
	const int MAX_LIGHTS = 10;

	struct PointLight
	{
		glm::vec4 position{};
		glm::vec4 color{}; // {red, green, blue, intensity}
	};

	struct GlobalUbo
	{
		glm::mat4 projection{1.f};
		glm::mat4 view{1.f};
		glm::vec4 ambientLightColor{1.f, 1.f, 1.f, .01f};
		PointLight pointLights[MAX_LIGHTS];
		int numLights;
	};

	struct FrameInfo
	{
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		EngineCamera& camera;
		VkDescriptorSet globalDescriptorSet;
		GameObject::Map& gameObject;
	};
} // namespace