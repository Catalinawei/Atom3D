#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include "Shader.h"
#include "InputManager.h"

namespace AtomToolKit {

	class Camera
	{
	public:
		Camera(Shader* shader, GLFWwindow* window);
		void Update(float deltaTime);
		void ProcessInput(GLFWwindow* window, float deltaTime);
		void Look(GLFWwindow* window, double xpos, double ypos);

		void SetWindowSize(int width, int height) {
			lastX = width / static_cast<float>(2); lastY = height / static_cast<float>(2);
		}

		glm::mat4 GetViewMatrix() const noexcept {
			return view;
		}

		glm::mat4 GetProjectionMatrix() const noexcept {
			return projection;
		}

		glm::vec3 GetCameraPos() const noexcept {
			return cameraPos;
		}

		glm::vec3 GetCameraFront() const noexcept {
			return cameraFront;
		}

		glm::vec3 GetCameraUp() const noexcept {
			return cameraUp;
		}

		constexpr Shader* GetCameraShader() const noexcept {
			return m_shader;
		}

		float FOV;
	private:
		glm::vec3 cameraPos;
		glm::vec3 cameraUp;
		glm::vec3 cameraFront;

		glm::mat4 view, projection;

		float pitch, yaw;
		float lastX, lastY;
		bool firstMouse;

		Shader* m_shader;
		GLFWwindow* m_window;
	};

}