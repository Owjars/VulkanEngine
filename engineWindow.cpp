#include "engineWindow.h"

#include <stdexcept>

namespace gameEngine
{

	EngineWindow::EngineWindow(uint32_t width, uint32_t height, std::string name) : width{ width }, height{ height }, windowName{ name }
	{
		initWindow();
	}

	EngineWindow::~EngineWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void EngineWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create window surface!");
		}
	}

	void EngineWindow::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	}

	void EngineWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto engWindow = reinterpret_cast<EngineWindow*>(glfwGetWindowUserPointer(window));
		engWindow->framebufferResized = true;
		engWindow->width = width;
		engWindow->height = height;
	}
} // namespace