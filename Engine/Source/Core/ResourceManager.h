#pragma once
#include "Sprite.h"
#include "OBJloader.h"
#include <unordered_map>

namespace AtomToolKit {

	class ResourceManager {
	public:

		ResourceManager(OBJloader* _loader) : loader(_loader)
		{
		}

		MeshData* getMesh(const char* path) {
			MeshData mesh_data;
			try {
				//load .obj and .mtl files
				//if a mesh has the same .obj, then skip loading again
				if (meshes[path].meshFile == path)
					return &meshes[path];

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
			//remove bc new takes heap time
			//GameObject* object = new GameObject();

			//std::cout << objects[name].meshFile << "\n";
			auto meshData = getMesh(objects[name].meshFile.c_str());
			if (!meshData) {
				std::cout << "mesh data is nullptr\n";
				return nullptr;
			}

			Material* objectMaterial = nullptr;
			if (!meshData->materialNames.empty()) {
				objectMaterial = getMaterial(meshData, meshData->materialNames[0].c_str());
			}
			objects[name].data = meshData;
			objects[name].material = objectMaterial;
			objects[name].vertices = meshData->vertices;
			return &objects[name];
		}

		auto& GetAllObjects(){
			return objects;
		}

	private:
		std::unordered_map <std::string, MeshData> meshes;
		std::unordered_map<std::string, Material> materials;
		std::unordered_map<std::string, GameObject> objects;

		OBJloader* loader;

	};

}