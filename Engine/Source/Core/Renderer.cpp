#include "Renderer.h"

using namespace AtomToolKit;

void Renderer::Render(Scene& scene) {
	auto lightingShader = scene.GetShader("LightingShader");
	auto lightingCubeShader = scene.GetShader("LightingCubeShader");
	auto directionalLight = scene.GetDirectionalLight();

	if (!lightingShader || !lightingCubeShader) {
		std::cerr << "RENDERER::ERROR shader is nullptr\n";
		return;
	}

	lightingShader->UseProgram();

	lightingShader->SetUniformVec3("directionalLight.ambient", directionalLight->ambient);
	lightingShader->SetUniformVec3("directionalLight.diffuse", directionalLight->diffuse);
	lightingShader->SetUniformVec3("directionalLight.specular", directionalLight->specular);

	lightingShader->SetUniformVec3("fogColor", glm::vec3(0.4f, 0.4f, 0.4f));
	lightingShader->SetUniformFloat("fogDensity", fogDensity);

	//lightPos = glm::vec3(sin(glfwGetTime()), 0.0f, cos(glfwGetTime()));

	lightingShader->SetUniformMatrix4("view", m_camera->GetViewMatrix());
	lightingShader->SetUniformMatrix4("projection", m_camera->GetProjectionMatrix());

	//lightingShader->SetUniformVec3("light.position", m_camera->GetCameraPos());
	//lightingShader->SetUniformVec3("light.direction", m_camera->GetCameraFront());
	lightingShader->SetUniformVec3("directionalLight.direction", glm::normalize(glm::vec3(0.3f, -0.25f, -0.9f)));
	lightingShader->SetUniformVec3("viewPos", m_camera->GetCameraPos());

	std::vector<glm::vec3> lightPositions = {
		glm::vec3(2.0f, 1.5f, 0.0f),   // LightSource1
		glm::vec3(-2.0f, 1.5f, -2.0f)  // LightSource2
	};

	lightingShader->SetUniformInt("pointLightSize", lightPositions.size());

	for (int i = 0; i < lightPositions.size(); i++) {
		std::string light = "pointLights[" + std::to_string(i) + "].";

		lightingShader->SetUniformVec3((light + "position").c_str(), lightPositions[i]);
		lightingShader->SetUniformFloat((light + "constant").c_str(), 1.0f);
		lightingShader->SetUniformFloat((light + "linear").c_str(), 0.09f);
		lightingShader->SetUniformFloat((light + "quadratic").c_str(), 0.032f);
		lightingShader->SetUniformVec3((light + "ambient").c_str(), pLight.ambient);
		lightingShader->SetUniformVec3((light + "diffuse").c_str(), pLight.diffuse);
		lightingShader->SetUniformVec3((light + "specular").c_str(), pLight.specular);
		//lightingShader->SetUniformFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
		//lightingShader->SetUniformFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
	}
	lightingShader->SetUniformVec3("spotLight.position", m_camera->GetCameraPos());
	lightingShader->SetUniformVec3("spotLight.direction", m_camera->GetCameraFront());
	lightingShader->SetUniformVec3("spotLight.ambient", glm::vec3(0.02f, 0.02f, 0.03f));
	lightingShader->SetUniformVec3("spotLight.diffuse", glm::vec3(1.0f, 0.95f, 0.85f));
	lightingShader->SetUniformVec3("spotLight.specular", glm::vec3(1.0f));
	lightingShader->SetUniformFloat("spotLight.constant", 1.0f);
	lightingShader->SetUniformFloat("spotLight.linear", 0.09f);
	lightingShader->SetUniformFloat("spotLight.quadratic", 0.032f);
	lightingShader->SetUniformFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	lightingShader->SetUniformFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

	for (auto i : scene.GetActors()) {
		
		if (!i) continue;

		auto spriteComponent = i->GetComponent<Sprite>();	
		auto spriteMaterial = i->GetComponent<Sprite>()->GetObjectMaterial();
		auto transformModel = i->GetComponent<Transform>()->GetModel();

		if (spriteComponent->GetShader() == lightingCubeShader) continue;

		lightingShader->SetUniformMatrix4("model", transformModel);
		lightingCubeShader->SetUniformMatrix4("model", transformModel);

		if (i->GetComponent<Sprite>()->HasMaterial()) {
			lightingShader->SetUniformVec3("material.ambient", spriteMaterial->ambient);
			lightingShader->SetUniformVec3("material.diffuse", spriteMaterial->diffusal);
			lightingShader->SetUniformVec3("material.specular", spriteMaterial->specular);
			lightingShader->SetUniformFloat("material.shininess", spriteMaterial->shininess);
			
			glActiveTexture(GL_TEXTURE0);

			if (spriteMaterial->diffuseTextureId) {
				glBindTexture(GL_TEXTURE_2D, spriteMaterial->diffuseTextureId);
				lightingShader->SetUniformInt("material.diffuseTexture", 0);  // <-- ADD THIS!
			}

		}

		spriteComponent->Render();
		//std::cout << "Updating spritess\n";
	}

	lightingCubeShader->UseProgram();

	lightingCubeShader->SetUniformMatrix4("view", m_camera->GetViewMatrix());
	lightingCubeShader->SetUniformMatrix4("projection", m_camera->GetProjectionMatrix());

	for (auto i : scene.GetActors()) {

		if (!i) continue;

		auto spriteComponent = i->GetComponent<Sprite>();
		auto spriteMaterial = i->GetComponent<Sprite>()->GetObjectMaterial();
		auto transformModel = i->GetComponent<Transform>()->GetModel();

		if (spriteComponent->GetShader() != lightingCubeShader) continue;

		lightingShader->SetUniformMatrix4("model", transformModel);
		lightingCubeShader->SetUniformMatrix4("model", transformModel);

		spriteComponent->Render();
		//std::cout << "Updating spritess\n";
	}

	//std::cout << "Updating renderer\n";
}