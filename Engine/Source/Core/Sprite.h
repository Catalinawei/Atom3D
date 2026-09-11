#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Component.h"
#include "Shader.h"
#include "OBJloader.h"
#include "btBulletDynamicsCommon.h"

using namespace AtomToolKit;

namespace AtomToolKit {

	struct Vertex {
		glm::vec3 vertices;
		glm::vec2 uv;
		glm::vec3 normals;
	};

	class Sprite : public Component
	{
	public:
		Sprite(Shader& shader);
		Sprite(Shader& shader, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& uvs,
			const std::vector<glm::vec3>& normals);

		void GenerateTextures(const char* texturePath);
		void SetMaterial(Material& mat) {
			m_material = mat;
			GenerateTextures(m_material.diffuseTexturePath.c_str());
		}
		void Update(float deltaTime) override;
		void Render();

		~Sprite() override;

		void HasMaterial(bool val) noexcept {
			hasMaterial = val;
		}

		bool enableTextures;

	public:
		//for renderer
		constexpr Material* GetObjectMaterial() noexcept {
			if (hasMaterial)
				return &m_material;
			return nullptr;
		}

		Shader* GetShader() const noexcept {
			return m_shader;
		}

		bool HasMaterial() const noexcept {
			return hasMaterial;
		}

		const std::vector<glm::vec3>& GetVertices() const noexcept {
			return m_vertices;
		}

		btVector3 GetHalfExtents(std::vector<glm::vec3>& vertices, const glm::vec3& scale);
		btVector3 GetCenter(std::vector<glm::vec3>& vertices, const glm::vec3& scale);

	private:
		void InitBuffers(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs, std::vector<glm::vec3> normals);

		unsigned int VBO; //store vertices
		unsigned int VAO;
		Shader* m_shader;
		std::vector<size_t> dataSize;

		Actor* m_owner;

		unsigned int Texture;
		int textureWidth, textureHeight, nrChannels;

		Material m_material;
		bool hasMaterial;

		std::vector<glm::vec3> m_vertices;
	};

}