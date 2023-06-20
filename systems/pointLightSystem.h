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

	class PointLightSystem
	{
	public:
		PointLightSystem(EngineDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~PointLightSystem();

		PointLightSystem(const PointLightSystem&) = delete;
		PointLightSystem& operator=(const PointLightSystem&) = delete;

		void update(FrameInfo& frameInfo, GlobalUbo& ubo);
		void render(FrameInfo& frameInfo);

	private:
		EngineDevice& engDevice;
		std::unique_ptr<EngPipeline> engPipeline;
		VkPipelineLayout pipelineLayout;

		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);
	};
} // namespace