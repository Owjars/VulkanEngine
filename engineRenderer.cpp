#include "engineRenderer.h"

#include <stdexcept>
#include <cassert>
#include <array>

namespace gameEngine
{

	EngineRenderer::EngineRenderer(EngineWindow& window, EngineDevice& device) : window{ window }, engDevice{ device }
	{
		recreateSwapChain();
		createCommandBuffers();
	}

	EngineRenderer::~EngineRenderer()
	{
		freeCommandBuffers();
	}

	void EngineRenderer::createCommandBuffers()
	{
		commandBuffers.resize(EngineSwapChain::MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = engDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(engDevice.getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate command buffers!");
		}
	}

	void EngineRenderer::freeCommandBuffers()
	{
		vkFreeCommandBuffers(engDevice.getDevice(), engDevice.getCommandPool(),
			static_cast<float>(commandBuffers.size()), commandBuffers.data());

		commandBuffers.clear();
	}

	void EngineRenderer::recreateSwapChain()
	{
		auto extent = window.getExtent();

		while (extent.width == 0 || extent.height == 0)
		{
			extent = window.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(engDevice.getDevice());

		//engSwapChain = nullptr;

		if (engSwapChain == nullptr)
		{
			engSwapChain = std::make_unique<EngineSwapChain>(engDevice, extent);
		}
		else
		{
			std::shared_ptr<EngineSwapChain> oldSwapChain = std::move(engSwapChain);
			engSwapChain = std::make_unique<EngineSwapChain>(engDevice, extent, oldSwapChain);

			if (!oldSwapChain->compareSwapFormats(*engSwapChain.get()))
			{
				throw std::runtime_error("Swap chain image format has changed!");
			}
		}
	}

	VkCommandBuffer EngineRenderer::beginFrame()
	{
		assert(!isFrameStarted && "Can't call beginFrame when already in progress");

		auto result = engSwapChain->acquireNextImage(&currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapChain();
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		isFrameStarted = true;

		auto commandBuffer = getCurrentCommandBuffer();

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("command buffer failed to begin recording!");
		}

		return commandBuffer;
	}

	void EngineRenderer::endFrame()
	{
		assert(isFrameStarted && "Can't call endFrame if frame is not in progress");

		auto commandBuffer = getCurrentCommandBuffer();

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record command buffer!");
		}

		auto result = engSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized())
		{
			window.resetWindowResizedFlag();
			recreateSwapChain();
		}
		else if (result != VK_SUCCESS)
		{
			throw std::runtime_error("failed to present swap chain image!");
		}

		isFrameStarted = false;

		currentFrameIndex = (currentFrameIndex + 1) & EngineSwapChain::MAX_FRAMES_IN_FLIGHT;
	}

	void EngineRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
		assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = engSwapChain->getRenderPass();
		renderPassInfo.framebuffer = engSwapChain->getFrameBuffer(currentImageIndex);
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = engSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(engSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(engSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, engSwapChain->getSwapChainExtent() };

		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void EngineRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
		assert(commandBuffer == getCurrentCommandBuffer() && "Can't end render pass on command buffer from a different frame");

		vkCmdEndRenderPass(commandBuffer);
	}
} // namespace