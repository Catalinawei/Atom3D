#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <vector>
#include "Actor.h"
#include "Shader.h"
#include "Sprite.h"

namespace AtomToolKit {
	class Engine
	{
	public:
		Engine();
		Engine(const char* windowTitle);
		~Engine();

		void Initiallize();
		virtual void Update();

		void changeWindowTitle(const char* name) {
			WINDOW_TITLE = name;
		}

	protected:
		std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> m_window;
		std::unique_ptr<Shader> m_shader;
	private:
		static constexpr int WINDOW_HEIGHT = 800;
		static constexpr int WINDOW_WIDTH = 800;
		const char* WINDOW_TITLE = "Atom3D";
		static constexpr bool FULLSCREEN = false;
		//"false" fullscreen but resizable
		static constexpr bool FALSE_FULLSCREEN = true;
	};
}

