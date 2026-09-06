#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace AtomToolKit {
	class InputManager
	{
	public:

		static bool KeyPressed(GLFWwindow* window, int key) {
			return glfwGetKey(window, key) ? true : false;
		}

		static void CursorPos(GLFWwindow* window, GLFWcursorposfun func) {
			glfwSetCursorPosCallback(window, func);
		}
	};

}
