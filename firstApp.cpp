#include "firstApp.h"

#include "engineCamera.h"
#include "keyboardMovementController.h"
#include "engineBuffer.h"
#include "engineFrameInfo.h"
#include "systems/simpleRenderSystem.h"
#include "systems/pointLightSystem.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <chrono>
#include <cassert>
#include <array>

namespace gameEngine
{
	FirstApp::FirstApp()
	{
		globalPool = EngineDescriptorPool::Builder(engDevice).setMaxSets(EngineSwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, EngineSwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();

		loadGameObjects();
	}

	FirstApp::~FirstApp() {}

	void FirstApp::run()
	{
		std::vector<std::unique_ptr<EngineBuffer>> uboBuffers(EngineSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < uboBuffers.size(); i++)
		{
			uboBuffers[i] = std::make_unique<EngineBuffer>(engDevice, sizeof(GlobalUbo), 1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			uboBuffers[i]->map();
		}

		auto globalSetLayout = EngineDescriptorSetLayout::Builder(engDevice)
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
			.build();

		std::vector<VkDescriptorSet> globalDescriptorSets(EngineSwapChain::MAX_FRAMES_IN_FLIGHT);

		for (int i = 0; i < globalDescriptorSets.size(); i++)
		{
			auto bufferInfo = uboBuffers[i]->descriptorInfo();

			EngineDescriptorWriter(*globalSetLayout, *globalPool)
				.writeBuffer(0, &bufferInfo)
				.build(globalDescriptorSets[i]);
		}

		SimpleRenderSystem simpleRenderSystem{ engDevice, engRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout() };
		PointLightSystem pointLightSystem{ engDevice, engRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout() };
		EngineCamera camera{};
		camera.setViewTarget(glm::vec3(-1.f, -2.5f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

		auto viewerObject = GameObject::createGameObject();
		viewerObject.transform.translation.z = -2.f;
		KeyboardMovementController cameraController{};

		auto currentTime = std::chrono::high_resolution_clock::now();

		while (!window.shouldClose())
		{
			glfwPollEvents();

			auto newTime = std::chrono::high_resolution_clock::now();

			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			frameTime = glm::min(frameTime, MAX_FRAME_TIME);

			cameraController.moveInPlaneXZ(window.getGLFWwindow(), frameTime, viewerObject);
			camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

			float aspect = engRenderer.getAspectRatio();
			camera.setPerspectiveProjection(glm::radians(50.f), aspect, .1f, 100.f);

			if (auto commandBuffer = engRenderer.beginFrame())
			{
				int frameIndex = engRenderer.getFrameIndex();

				FrameInfo frameInfo
				{
					frameIndex,
					frameTime,
					commandBuffer,
					camera,
					globalDescriptorSets[frameIndex],
					gameObjects
				};

				// update
				GlobalUbo ubo{};
				ubo.projection = camera.getProjection();
				ubo.view = camera.getView();
				pointLightSystem.update(frameInfo, ubo);
				uboBuffers[frameIndex]->writeToBuffer(&ubo);
				uboBuffers[frameIndex]->flush();

				// render
				engRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(frameInfo);
				pointLightSystem.render(frameInfo);
				engRenderer.endSwapChainRenderPass(commandBuffer);
				engRenderer.endFrame();
			}

			vkDeviceWaitIdle(engDevice.getDevice());
		}
	}

	void FirstApp::loadGameObjects()
	{
		std::shared_ptr<EngineModel> engModel = EngineModel::createModelFromFile(engDevice, "models/flat_vase.obj");

		auto flatVase = GameObject::createGameObject();
		flatVase.model = engModel;
		flatVase.transform.translation = { .5f, .5f, 0.f };
		flatVase.transform.scale = glm::vec3{ 3.f, 1.5f, 3.f };

		gameObjects.emplace(flatVase.getId(), std::move(flatVase));


		engModel = EngineModel::createModelFromFile(engDevice, "models/smooth_vase.obj");

		auto smoothVase = GameObject::createGameObject();
		smoothVase.model = engModel;
		smoothVase.transform.translation = { -.5f, .5f, 0.f };
		smoothVase.transform.scale = glm::vec3{ 3.f, 1.5f, 3.f };

		gameObjects.emplace(smoothVase.getId(), std::move(smoothVase));


		engModel = EngineModel::createModelFromFile(engDevice, "models/quad.obj");

		auto floor = GameObject::createGameObject();
		floor.model = engModel;
		floor.transform.translation = { 0.f, .5f, 0.f };
		floor.transform.scale = glm::vec3{ 3.f, 1.f, 3.f };
		gameObjects.emplace(floor.getId(), std::move(floor));

		std::vector<glm::vec3> lightColors
		{
			{ 1.f, .1f, .1f },
			{ .1f, .1f, 1.f },
			{ .1f, 1.f, .1f },
			{ 1.f, 1.f, .1f },
			{ .1f, 1.f, 1.f },
			{ 1.f, 1.f, 1.f },
		};

		for (int i = 0; i < lightColors.size(); i++)
		{
			auto pointLight = GameObject::makePointLight(.2f);
			pointLight.color = lightColors[i];
			auto rotateLight = glm::rotate(
				glm::mat4(1.f),
				(i * glm::two_pi<float>()) / lightColors.size(),
				{ 0.f, -1.f, 0.f });

			pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
			gameObjects.emplace(pointLight.getId(), std::move(pointLight));
		}
	}
} // namespace