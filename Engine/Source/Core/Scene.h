#pragma once
#include "Actor.h"
#include "UI.h"
#include "OBJloader.h"
#include "ResourceManager.h"
#include "Transform.h"
#include "MemoryPool.h"
#include "Skybox.h"
#include "PhysicsManager.h"
#include "PhysicsComponent.h"

namespace AtomToolKit {

	struct Light {
		glm::vec3 position = glm::vec3(0.0f);

		glm::vec3 ambient = glm::vec3(0.2f);
		glm::vec3 diffuse = glm::vec3(1.0f);
		glm::vec3 specular = glm::vec3(1.0f);
	};

	class Scene
	{
	public:
		Scene(ResourceManager* __rManager);
		void Initiallize(const char* scenePat, const char* skyboxVShader, const char* skyboxFShader);
		void Update(float deltaTime);
		void Render();

		void SetupShaders(const char* shaderName, std::unique_ptr<Shader> data) {
			object_Shaders[shaderName] = std::move(data);
		}
	public:
		constexpr Light* GetDirectionalLight() {
			return &m_directionalLight;
		}

		Shader* GetShader(const std::string& sName) {

			auto i = object_Shaders.find(sName);
			if (i != object_Shaders.end()) return i->second.get();
			std::cerr << "shader does not exist\n";
			return nullptr;
		}

		SkyBox* GetSkyBox() const noexcept {
			return m_defaultSkybox.get();
		}

		std::vector<Actor*> GetActors();

	private:

		std::unordered_map<std::string, std::unique_ptr<Shader>> object_Shaders;
		MemoryPool<Actor> m_actors;
		ResourceManager* resourceManager;
		std::unique_ptr<Shader> m_skyboxShader;
		std::unique_ptr<SkyBox> m_defaultSkybox;
		std::unique_ptr<PhysicsManager> m_physicsManager;

		Light m_directionalLight;
		size_t lightId;
	};

}