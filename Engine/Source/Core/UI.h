#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm.hpp>

namespace AtomToolKit {
	class UI
	{
	public:
		UI(GLFWwindow* mainWindow);

		void Update(float deltaTime);

		void Render();

		~UI();

		bool DrawWindow(const char* _windowTitle) const {
			if (ImGui::Begin(_windowTitle)) return true;
			return false;
		}

		void drawText(const char* _message, float _fontSize) const {
			ImGui::PushFont(NULL, _fontSize);
			ImGui::Text(_message);
			ImGui::PopFont();
		}

		void DrawSliderFloat(const char* message, float& var, float min_value, float max_value) const {
			ImGui::SliderFloat(message, &var, min_value, max_value);
		}

		void DrawCheckBox(const char* message, bool& var) const {
			ImGui::Checkbox(message, &var);
		}

		void DrawColorPallete3(const char* msg, glm::vec3& var) const {
			ImGui::ColorEdit3(msg, &var[0]);
		}


		bool DrawAndPressedButton(const char* _buttonMessage) {
			//return true if pressed
			if (ImGui::Button(_buttonMessage)) return true;
			return false;
		}

	private:
		GLFWwindow* window;
		void Initiallize();
	};

}