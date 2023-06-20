#pragma once

#include "engineWindow.h"
#include "engineDevice.h"
#include "engineGameObject.h"
#include "engineRenderer.h"
#include "engineDescriptors.h"

#include <memory>
#include <vector>

namespace gameEngine
{

	class FirstApp
	{
	public:
		static constexpr uint32_t WIDTH = 800;
		static constexpr uint32_t HEIGHT = 600;
		static constexpr float MAX_FRAME_TIME = 2.f;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;

		void run();

	private:
		EngineWindow window{ WIDTH, HEIGHT, "Vulkan" };
		EngineDevice engDevice{ window };
		EngineRenderer engRenderer{ window, engDevice };

		std::unique_ptr<EngineDescriptorPool> globalPool;
		GameObject::Map gameObjects;

		void loadGameObjects();
	};
} // namespace