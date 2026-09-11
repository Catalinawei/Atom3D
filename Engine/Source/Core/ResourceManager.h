#pragma once
#include "Sprite.h"
#include "OBJloader.h"
#include <unordered_map>

namespace AtomToolKit {

	struct MeshData {
		std::vector<glm::vec3> vertices, normals;
		std::vector<glm::vec2> uv;
		std::string materialFileName;
		std::vector<std::string> materialNames;

		std::unordered_map<std::string, Material> m_materials;
	};

	struct GameObject {
		glm::vec3 position = glm::vec3(1.0f);
		glm::vec3 rotation = glm::vec3(1.0f);
		glm::vec3    scale = glm::vec3(1.0f);

		MeshData* data = nullptr;

		Material* material;

		float mass = 1.0f;

		std::vector<glm::vec3> vertices;

		std::vector<std::string> componentNames;
		std::vector<Component*> components;
	};

	class ResourceManager {
	public:

		ResourceManager(OBJloader* _loader) : loader(_loader)
		{
		}

		MeshData* getMesh(const char* path) {
			MeshData mesh_data;
			try {
				//load .obj and .mtl files
				OBJloader::LoadObject(path, mesh_data.vertices, mesh_data.uv, mesh_data.normals
					, mesh_data.materialFileName);

				//if(!mesh_data.materialNames.empty())
				mesh_data.m_materials = loader->LoadMTLFile(mesh_data.materialFileName.c_str(), mesh_data.materialNames);

				meshes[path] = mesh_data;

				std::cout << "Correctly loaded mesh data!\n";
			}
			catch (std::exception& e) {
				std::cerr << e.what() << '\n';
				std::cout << "Error at loading object, Resource Manager\n";
				return nullptr;
			}
			return &meshes[path];
		}

		Material* getMaterial(MeshData* data, const char* materialName) {
			if (!data || data->m_materials.empty()) {
				std::cout << "RESOURCEMANAGER: returned nullptr\n";
				return nullptr;
			}

			auto it = data->m_materials.find(materialName);
			if (it != data->m_materials.end() && !data->m_materials.empty()) return &it->second;

			std::cout << "returned nullptr \n";
			return nullptr;
		}

		void LoadScene(const char* sceneName) {
			try {
				objects = loader->LoadScene(sceneName);
			}
			catch (std::exception& e) {
				std::cout << e.what() << "\n";
			}
		}

		GameObject* getObject(const char* name) {
			GameObject* object = new GameObject();

			std::cout << objects[name].meshFile << "\n";
			auto meshData = getMesh(objects[name].meshFile.c_str());
			if (!meshData) {
				std::cout << "mesh data is nullptr\n";
				return nullptr;
			}

			Material* objectMaterial = nullptr;
			if (!meshData->materialNames.empty()) {
				objectMaterial = getMaterial(meshData, meshData->materialNames[0].c_str());
			}
			object->data = meshData;
			object->position = objects[name].position;
			object->rotation = objects[name].rotation;
			object->scale = objects[name].scale;
			object->material = objectMaterial;
			object->mass = objects[name].mass;
			object->vertices = meshData->vertices;
			object->components = objects[name].components;
			object->componentNames = objects[name].componentNames;
			return object;
		}

		const std::unordered_map<std::string, Object>& GetAllObjects() const {
			return objects;
		}

	private:
		std::unordered_map <std::string, MeshData> meshes;
		std::unordered_map<std::string, Material> materials;
		std::unordered_map<std::string, Object> objects;

		OBJloader* loader;

	};

}