#pragma once
#include "Scene.h"
#include "Camera.h"

namespace AtomToolKit {

	struct PointLight {
		glm::vec3 ambient = glm::vec3(0.05f);
		glm::vec3 diffuse = glm::vec3(0.8f);
		glm::vec3 specular = glm::vec3(1.0f);
	};

	class Renderer
	{
	public:
		void Render(Scene& Scene);

		Renderer(Camera* camera)
			: m_camera(camera) {
		}

		float fogDensity = 0.01f;

	private:
		Camera* m_camera;

		PointLight pLight;
	};

}