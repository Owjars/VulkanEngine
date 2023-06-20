#pragma once

#include "engineWindow.h"

#include <string>
#include <vector>
 
namespace gameEngine
{
	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct QueueFamilyIndices
	{
		uint32_t graphicsFamily;
		uint32_t presentFamily;
		bool graphicsFamilyHasValue;
		bool presentFamilyHasValue;

		bool isComplete()
		{
			return graphicsFamilyHasValue && presentFamilyHasValue;
		}
	};

	class EngineDevice
	{
	public:
#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif //NDEBUG

		EngineDevice(EngineWindow& window);
		~EngineDevice();

		// Not copyable or movable
		EngineDevice(const EngineDevice&) = delete;
		EngineDevice& operator=(const EngineDevice&) = delete;
		EngineDevice(EngineDevice&&) = delete;
		EngineDevice& operator=(EngineDevice&&) = delete;

		VkCommandPool getCommandPool() { return commandPool; }
		VkDevice getDevice() { return engDevice; }
		VkSurfaceKHR getSurface() { return surface; }
		VkQueue getGraphicsQueue() { return graphicsQueue; }
		VkQueue getPresentQueue() { return presentQueue; }

		SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice); }
		QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice); }

		VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

		VkCommandBuffer beginSingleTimeCommands();

		void endSingleTimeCommands(VkCommandBuffer commandBuffer);

		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

		void createImageWithInfo(const VkImageCreateInfo& imageInfo, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

		VkPhysicalDeviceProperties properties;

	private:
		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		EngineWindow& window;
		VkCommandPool commandPool;

		VkDevice engDevice;
		VkSurfaceKHR surface;
		VkQueue graphicsQueue;
		VkQueue presentQueue;

		const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
		const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		void createInstance();
		void setupDebugMessenger();
		void createSurface();
		void pickPhysicalDevice();
		void createLogicalDevice();
		void createCommandPool();

		bool isDeviceSuitable(VkPhysicalDevice device);

		std::vector<const char*> getRequiredExtensions();

		bool checkValidationLayerSupport();

		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		void hasGlfwRequiredInstanceExtensions();

		bool checkDeviceExtensionSupport(VkPhysicalDevice device);

		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	};
} // namespace