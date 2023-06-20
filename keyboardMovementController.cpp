#include "keyboardMovementController.h"

namespace gameEngine
{

	void KeyboardMovementController::moveInPlaneXZ(GLFWwindow* window, float dt, GameObject& targetObject)
	{
		glm::vec3 rotate{0.f};

		if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1.f;
		if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.f;
		if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x += 1.f;
		if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x -= 1.f;

		if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
		{
			targetObject.transform.rotation += turnSpeed * dt * glm::normalize(rotate);
		}

		targetObject.transform.rotation.x = glm::clamp(targetObject.transform.rotation.x, -1.5f, 1.5f);
		targetObject.transform.rotation.y = glm::mod(targetObject.transform.rotation.y, glm::two_pi<float>());

		float yaw = targetObject.transform.rotation.y;
		const glm::vec3 forwardDirection{sin(yaw), 0.f, cos(yaw)};
		const glm::vec3 rightDirection{forwardDirection.z, 0.f, -forwardDirection.x};
		const glm::vec3 upDirection{0.f, -1.f, 0.f};

		glm::vec3 movDirection{0.f};
		if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) movDirection += forwardDirection;
		if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) movDirection -= forwardDirection;
		if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) movDirection += rightDirection;
		if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) movDirection -= rightDirection;
		if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) movDirection += upDirection;
		if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) movDirection -= upDirection;

		if (glm::dot(movDirection, movDirection) > std::numeric_limits<float>::epsilon())
		{
			targetObject.transform.translation += moveSpeed * dt * glm::normalize(movDirection);
		}
	}
} // namespace