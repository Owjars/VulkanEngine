#pragma once

#include "engineModel.h"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <unordered_map>

namespace gameEngine
{

	struct Transform2DComponent
	{
		glm::vec2 translation{};
		glm::vec2 scale{1.f, 1.f};
		float rotation;

		glm::mat2 mat2()
		{
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);
			glm::mat2 rotMat{{c, s}, { -s, c }};

			glm::mat2 scaleMat{{scale.x, 0.f}, { .0f, scale.y }};
			return rotMat * scaleMat;
		}
	};

	struct TransformComponent
	{
		glm::vec3 translation{};
		glm::vec3 scale{1.f, 1.f, 1.f};
		glm::vec3 rotation{};

		// Transform order: translate * Ry * Rx * Rz * scale
		glm::mat4 mat4();
		glm::mat3 normalMatrix();
	};

	struct PointLightComponent
	{
		float lightIntensity = 1.f;
	};

	class GameObject
	{
	public:
		using id_t = unsigned int;
		using Map = std::unordered_map<id_t, GameObject>;

		glm::vec3 color{};
		TransformComponent transform{};
		
		// Optional
		std::shared_ptr<EngineModel> model{};
		std::unique_ptr<PointLightComponent> pointLight = nullptr;

		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject(GameObject&&) = default;
		GameObject& operator=(GameObject&&) = default;

		static GameObject createGameObject()
		{
			static id_t currentId = 0;
			return GameObject(currentId++);
		}

		static GameObject makePointLight(float intensity = 5.f, float radius = .1f, glm::vec3 color = glm::vec3{ 1.f });

		id_t getId() { return id; }

	private:
		GameObject(id_t objId) : id{ objId } {};

		id_t id;
	};
} // namespace