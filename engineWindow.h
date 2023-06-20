#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace gameEngine
{

	class EngineWindow
	{
	public:
		EngineWindow(uint32_t width, uint32_t height, std::string name);
		~EngineWindow();

		EngineWindow(const EngineWindow&) = delete;
		EngineWindow& operator=(const EngineWindow&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); }

		VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }

		bool wasWindowResized() { return framebufferResized; }

		void resetWindowResizedFlag() { framebufferResized = false; }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

		GLFWwindow* getGLFWwindow() const { return window; }

	private:
		GLFWwindow* window;

		uint32_t width;
		uint32_t height;
		bool framebufferResized = false;

		std::string windowName;

		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

		void initWindow();
	};
} // namespace