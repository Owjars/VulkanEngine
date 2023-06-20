#pragma once

#include "../engineFrameInfo.h"
#include "../engPipeline.h"
#include "../engineDevice.h"
#include "../engineGameObject.h"
#include "../engineCamera.h"

#include <memory>
#include <vector>

namespace gameEngine
{

	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(EngineDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void renderGameObjects(FrameInfo& frameInfo);

	private:
		EngineDevice& engDevice;
		std::unique_ptr<EngPipeline> engPipeline;
		VkPipelineLayout pipelineLayout;

		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);
	};
} // namespace