#include "UI.h"

using namespace AtomToolKit;

UI::UI(GLFWwindow* mainWindow) : window(mainWindow){
	Initiallize();
}

void UI::Initiallize() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	auto io = ImGui::GetIO();

	//configurations for imgui
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.WantCaptureMouse = true;
	
	// initallize glfw and opengl 330 core for imgui
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void UI::Update(float deltaTime) {
	//main loop calls poll events
	
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	//draw
	//ImGui::ShowDebugLogWindow();
}

void UI::Render() {
	//render after opengl things
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//ImGui::End();
}

UI::~UI() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}