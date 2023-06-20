#pragma once

#include "engineWindow.h"
#include "engineDevice.h"
#include "engineSwapchain.h"

#include <memory>
#include <vector>
#include <cassert>

namespace gameEngine
{

	class EngineRenderer
	{
	public:
		EngineRenderer(EngineWindow& window, EngineDevice& device);
		~EngineRenderer();

		EngineRenderer(const EngineRenderer&) = delete;
		EngineRenderer& operator=(const EngineRenderer&) = delete;

		VkRenderPass getSwapChainRenderPass() const { return engSwapChain->getRenderPass(); }
		float getAspectRatio() const { return engSwapChain->extentAspectRatio(); }

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

		bool isFrameInProgress() const { return isFrameStarted; }

		int getFrameIndex() const
		{
			assert(isFrameStarted && "Cannot get frame index when frame not in progress");
			return currentFrameIndex;
		}

		VkCommandBuffer getCurrentCommandBuffer() const
		{
			assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
			return commandBuffers[currentFrameIndex];
		}

	private:
		EngineWindow& window;
		EngineDevice& engDevice;
		std::unique_ptr<EngineSwapChain> engSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex{ 0 };
		bool isFrameStarted = false;

		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();
	};
} // namespace