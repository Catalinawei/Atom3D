#include "Game.h"
#include <filesystem>

static Game* gameInstance = nullptr;

void Game::ApplyMouseLock(bool locked) {
	lockedMouse = locked;

	if (locked) {
		glfwSetInputMode(m_window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		int width, height;
		glfwGetWindowSize(m_window.get(), &width, &height);
		glfwSetCursorPos(m_window.get(), width / 2.0, height / 2.0);
	}
	else {
		glfwSetInputMode(m_window.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

Game::Game() : Engine("Custom Counter Strike") {
	Initiallize();
}

void Game::Initiallize() {

	gameInstance = this;

	m_lastTime = static_cast<float>(glfwGetTime());
	m_currentTime = 0.0f;
	m_deltaTime = 0.0f;

	//m_shader = std::make_unique<Shader>("Shaders/vertexShader.glsl", "Shaders/TextureFragmentShader.glsl");

	m_resourceManager = std::make_unique<ResourceManager>(m_loader.get());
	m_gui = std::make_unique<UI>(m_window.get());
	m_loader = std::make_unique<OBJloader>();

	{
		auto m_lightingShader = std::make_unique<Shader>("Assets/Shaders/LightingVertexShader.glsl", "Assets/Shaders/LightingFragmentShader.glsl");
		auto m_lightingCubeShader = std::make_unique<Shader>("Assets/Shaders/LightingVertexShader.glsl", "Assets/Shaders/LightingFragmentShader.glsl");

		m_mainScene = std::make_unique<Scene>(m_resourceManager.get());
		m_mainScene->SetupShaders("LightingShader", std::move(m_lightingShader));
		m_mainScene->SetupShaders("LightingCubeShader", std::move(m_lightingCubeShader));
	}

	m_camera = std::make_unique<Camera>(m_shader.get(), m_window.get());

	m_mainRenderer = std::make_unique<Renderer>(m_camera.get());

	m_mainScene->Initiallize("Assets/Scenes/Scene.sc", m_camera.get(), m_window.get());

	//cursor things
	auto cursorLambda = [](GLFWwindow* window, double xpos, double ypos) {
		ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);

		if (gameInstance && gameInstance->m_camera && gameInstance->lockedMouse) {
			gameInstance->m_camera->Look(window, xpos, ypos);
		}
	};
	lightColorDiffuse = glm::vec3(1.0f);

	std::vector<std::string> skyboxTextures = {
		"Assets/Skybox/LearnOpengl/right.jpg",
		"Assets/Skybox/LearnOpengl/left.jpg",
		"Assets/Skybox/LearnOpengl/top.jpg",
		"Assets/Skybox/LearnOpengl/bottom.jpg",
		"Assets/Skybox/LearnOpengl/front.jpg",
		"Assets/Skybox/LearnOpengl/back.jpg"
	};
	m_mainScene->SetupSkybox(skyboxTextures, "Assets/Shaders/SkyboxVertexShader.glsl", "Assets/Shaders/SkyboxFragmentShader.glsl");

	glfwSetCursorPosCallback(m_window.get(), cursorLambda);
	//glfwSetInputMode(m_window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	//disable vsync
	glfwSwapInterval(0);

	//std::cout << m_mainScene->GetShader("LightingShader")->GetCode(GL_FRAGMENT_SHADER) << "\n";
	InitiallizeObjects();
}

void Game::InitiallizeObjects() {

	GLFWimage windowIcon;
	unsigned char* data = stbi_load("Assets/Window/Icon.png", &windowIcon.width, &windowIcon.height, 0, 4);
	windowIcon.pixels = data;

	glfwSetWindowIcon(m_window.get(), 1, &windowIcon);

	stbi_image_free(data);

	//SDL makes my engine freeze 4 to 5 seconds, maybe later i will build async later(multiple loops)
	//TODO: Async (maybe later)
	auto initializeLambda = []() {
		gameInstance->m_audio = std::make_unique<Audio>();
	};
	//launch policy = always on another thread hahahah (because audio takes time to initiallize)
	initializationThread = std::async(std::launch::async, initializeLambda);
}

void Game::Update() {
	bool soundGood = false;
	while (!glfwWindowShouldClose(m_window.get())) {
		//if initialization is valid, then wait 0 seconds to check if ready (non blocking async)
		if (initializationThread.valid()) {
			//std::cout << "Future is valid\n";
			auto status = initializationThread.wait_for(std::chrono::seconds(0));
			if (status == std::future_status::ready) {
				initializationThread.get();
				soundGood = true;
			}
		}

		//if (soundGood) {
		//	Sound sound;
		//	gameInstance->m_audio->loadWavFile("Assets/Audio/unhappy.wav", sound);
		//	gameInstance->m_audio->ChangeVolume(sound, AUDIO_S32LSB, 1);//max volume = 128
		//	gameInstance->m_audio->PlaySimpleAudio(sound);
		//	soundGood = false;
		//}

		//check for any events 
		glfwPollEvents();

		m_gui->Update(m_deltaTime);

		glClearColor(0.0f, 0.0f, 0.0f, .0f);
		//clears the entire color buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_mainScene->GetSkyBox()->Render(*m_camera);

		//heart of the engine
		m_currentTime = (float)glfwGetTime();
		m_deltaTime = m_currentTime - m_lastTime;
		m_lastTime = m_currentTime;

		m_camera->Update(m_deltaTime);
		m_camera->ProcessInput(m_window.get(), m_deltaTime);
		ProcessInput(m_deltaTime);

		m_mainScene->Update(m_deltaTime);
		m_mainRenderer->Render(*m_mainScene);

		if (m_gui->DrawWindow("Debug Window")) {
			float fps = 1 / m_deltaTime;
			std::string fpsMessage = "FPS: " + std::to_string(fps);
			m_gui->drawText(fpsMessage.c_str(), 20.0f);

			m_gui->drawText(lockedMouse ? "mouse locked" : "mouse unlocked", 20.0f);

			m_gui->DrawSliderFloat("FOV", m_camera->FOV, 45.0f, 120.0f);
			
			m_gui->DrawSliderFloat("Fog Density", m_mainRenderer->fogDensity, 0.01f, 0.5f);
			//m_gui->DrawColorPallete3("Light Color", m_mainScene->GetLightObject()->diffuse);
		}

		ImGui::End();

		if (m_gui->DrawWindow("Actors")) {
			for (auto i : m_mainScene->GetActors()){
				m_gui->drawText(i->GetName().c_str(), 20.0f);
			}
		}

		ImGui::End();

		m_gui->Render();

		Engine::Update();
	}
}

void Game::ProcessInput(float deltaTime) {
	bool keyDown = glfwGetKey(m_window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS;
	if (keyDown && !keyPress) {
		ToggleMouseLock();
	}
	keyPress = keyDown;
}
