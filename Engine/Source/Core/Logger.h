#pragma once
#include <glm.hpp>
#include <iostream>
#include "UI.h"
#include <string>

namespace AtomToolKit {
	class Logger
	{
	public:
		Logger(UI* gui)
			: m_ui(gui) {
		}

		template<typename T>
		void Log(const T& msg) {
			//m_ui->DrawText(msg, 20.0f);
		}

		void DrawLogWindow(UI& gui) {
			if (gui.DrawWindow("Log")) {
				Log("Engine v2.0");
				Log("Using GLFW/OpenGL #version 330 core");
			}

			ImGui::End();
		}
	private:
		UI* m_ui;
	};

}