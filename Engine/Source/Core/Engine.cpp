#include "Engine.h"

using namespace AtomToolKit;

Engine::Engine() : m_window(nullptr, glfwDestroyWindow){
	try {
		Initiallize();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << "\n";
	}

}

void Engine::Initiallize() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	
	{
		auto windowMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		m_window.reset(glfwCreateWindow(FALSE_FULLSCREEN ? windowMode->width : WINDOW_WIDTH,
			FALSE_FULLSCREEN ? windowMode->height : WINDOW_HEIGHT, WINDOW_TITLE,
			FULLSCREEN ? glfwGetPrimaryMonitor() : nullptr, nullptr));
	}

	if (!m_window) {
		throw std::runtime_error("Failed to create window");
		glfwTerminate();
	}

	//make m_window the context for the current thread
	glfwMakeContextCurrent(m_window.get());

	//load opengl functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("Failed to load gl functions");
	}

	//Tell opengl the dimensions to display things
	{
		int viewportWidth, viewportHeight;
		glfwGetFramebufferSize(m_window.get(), &viewportWidth, &viewportHeight);
		glViewport(0, 0, viewportWidth, viewportHeight);
	}

	m_shader = std::make_unique<Shader>(
		"E:/visualStudioProjects/3DGame/3DGame/Shaders/vertexShader.glsl",
		"E:/visualStudioProjects/3DGame/3DGame/Shaders/TexturefragmentShader.glsl"
	);
}

void Engine::Update(){
	glfwSwapBuffers(m_window.get());
}

Engine::~Engine() {
	glfwTerminate();
}