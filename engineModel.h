#pragma once

#include "engineDevice.h"
#include "engineBuffer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace gameEngine
{

	class EngineModel
	{
	public:
		struct Vertex
		{
			glm::vec3 position{};
			glm::vec3 color{};
			glm::vec3 normal{};
			glm::vec2 uv{};

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

			bool operator==(const Vertex& other) const
			{
				return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
			}
		};

		struct Builder
		{
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};

			void loadModel(const std::string& filepath);
		};

		EngineModel(EngineDevice& device, const EngineModel::Builder& builder);
		~EngineModel();

		EngineModel(const EngineModel&) = delete;
		EngineModel& operator=(const EngineModel&) = delete;

		static std::shared_ptr<EngineModel> createModelFromFile(EngineDevice& device, const std::string& filepath);

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private:
		EngineDevice& engDevice;

		std::unique_ptr<EngineBuffer> vertexBuffer;
		uint32_t vertexCount;

		bool hasIndexBuffer;
		std::unique_ptr<EngineBuffer> indexBuffer;
		uint32_t indexCount;

		void createVertexBuffers(const std::vector<Vertex>& vertices);
		void createIndexBuffers(const std::vector<uint32_t>& indices);
	};
} // namespace