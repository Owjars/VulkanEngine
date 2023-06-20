#pragma once

#include "engineDevice.h"

#include <string>
#include <vector>

namespace gameEngine
{

	struct PipelineConfigInfo
	{
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

		std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
		VkPipelineViewportStateCreateInfo viewportInfo{};
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
		VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
		VkPipelineMultisampleStateCreateInfo multisampleInfo{};
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
		std::vector<VkDynamicState> dynamicStateEnables{};
		VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};


	class EngPipeline
	{
	public:
		EngPipeline(EngineDevice& device, const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);

		~EngPipeline();

		EngPipeline(const EngPipeline&) = delete;
		EngPipeline& operator=(const EngPipeline&) = delete;

		static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

		void bind(VkCommandBuffer commandBuffer);

	private:
		static std::vector<char> readFile(const std::string& filepath);
		EngineDevice& engDevice;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;

		void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);

		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
	};
} // namespace