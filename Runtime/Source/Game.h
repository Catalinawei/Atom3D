#pragma once

#include "Core/Engine.h"
#include "Core/Transform.h"
#include "Core/OBJloader.h"
#include "Core/Camera.h"
#include "Core/ResourceManager.h"
#include "Core/UI.h"
#include "Core/Renderer.h"
#include "Core/Scene.h"
#include "Core/Logger.h"
#include "Core/Audio.h"

#include <future>
#include <chrono>

using namespace AtomToolKit;

class Game :
    public Engine
{
public:
    Game();
    void Initiallize();

    void Update() override;

private:

    void InitiallizeObjects();

    std::unique_ptr<ResourceManager> m_resourceManager;
    std::unique_ptr<OBJloader> m_loader;
    std::unique_ptr<UI> m_gui;
    std::unique_ptr<Camera> m_camera;
    std::unique_ptr<Scene> m_mainScene;
    std::unique_ptr<Renderer> m_mainRenderer;
    std::unique_ptr<Audio> m_audio;
    
    //async stuff
    std::future<void> initializationThread;

    //std::unique_ptr<Shader> m_lightingShader, m_lightingCubeShader;

    float m_lastTime;
    float m_currentTime;
    float m_deltaTime;

private:

    void ProcessInput(float deltaTime);
    void ApplyMouseLock(bool locked);
    void ToggleMouseLock() {
        ApplyMouseLock(!lockedMouse);
    }

    bool lockedMouse = false;
    bool keyPress = false;

private:
    size_t lightId;
    glm::vec3 lightPos;

    glm::vec3 lightColorDiffuse;
};

