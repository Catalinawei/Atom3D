#pragma once
#include <fstream>
#include <sstream>
#include <exception>

#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <unordered_map>

namespace AtomToolKit {

	struct Material {
		std::string name = "Unnamed";

		glm::vec3 ambient = glm::vec3(0.1f);
		glm::vec3 diffusal = glm::vec3(1.0f);
		glm::vec3 specular = glm::vec3(0.2f);
		float shininess = 32.0f;

		unsigned int diffuseTextureId = 0;
		std::string diffuseTexturePath = "Unnamed";
		bool hasTexture{ false };
	};

	struct Object {
		std::string name = "unnamed";

		glm::vec3 position = glm::vec3(1.0f);
		glm::vec3 rotation = glm::vec3(1.0f);
		glm::vec3 scale = glm::vec3(1.0f);

		std::string vertexShaderFile = "Unnamed.glsl";
		std::string fragmentShaderFile = "Unnamed.glsl";
		std::string meshFile = "Unnamed.obj";

		//physics
		float mass = 1.0f;

		//aditional components
		std::vector<Component*> components;
		std::vector<std::string> componentNames;
	};

	class OBJloader
	{
	public:

		static void LoadObject(const char* path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvCoordinates,
			std::vector<glm::vec3>& out_normals, std::string& mat_name) {
			std::vector<unsigned int> vertexIndices, uvCoordinatesIndices, normalIndices;
			std::vector<glm::vec3> temp_vertices, temp_normals;
			std::vector<glm::vec2> temp_uvCoordinates;

			auto file = std::make_unique<std::ifstream>(path);
			if (!file) {
				throw std::runtime_error("Couldnt load object file!");
			}

			if (!file->is_open()) {
				std::cout << "FILE NOT FOUND!\n";
			}

			std::string lineHeader;

			while (std::getline(*file, lineHeader)) {

				std::istringstream iss(lineHeader);
				std::string token; iss >> token;

				//if the file is empty, break the loop
				//if (token == EOF) break;

				//.mat files
				if (token == "mtllib") {
					iss >> mat_name;
				}

				//vertices/vertex
				if (token == "v") {
					glm::vec3 vertex;
					iss >> vertex.x >> vertex.y >> vertex.z;
					temp_vertices.push_back(vertex);
				}
				//uv coordinates
				else if (token == "vt") {
					glm::vec2 uv;
					iss >> uv.x >> uv.y;
					temp_uvCoordinates.push_back(uv);

				}
				//normals
				else if (token == "vn") {
					glm::vec3 normals;
					iss >> normals.x >> normals.y >> normals.z;
					temp_normals.push_back(normals);
				}
				else if (token == "f") {
					std::string vertexData;
					unsigned int vertices[3], normals[3], uv[3];
					int vertexCount = 0; //more automaitc

					while (iss >> vertexData) {
						//replace / for [space] for istringstream to parse it
						std::replace(vertexData.begin(), vertexData.end(), '/', ' ');
						std::istringstream viss(vertexData);

						viss >> vertices[vertexCount] >> uv[vertexCount] >> normals[vertexCount];
						vertexCount++;
					}

					for (auto i = 0; i < 3; i++) {
						vertexIndices.push_back(vertices[i]);
						normalIndices.push_back(normals[i]);
						uvCoordinatesIndices.push_back(uv[i]);
					}
				}
			}
			for (unsigned int i = 0; i < vertexIndices.size(); i++) {
				unsigned int vertexIndex = vertexIndices[i];
				glm::vec3 vertex = temp_vertices[vertexIndex - 1];
				out_vertices.push_back(vertex);
				//std::cout << "vertex successful!\n";
			}
			for (unsigned int i = 0; i < uvCoordinatesIndices.size(); i++) {
				unsigned int uvIndex = uvCoordinatesIndices[i];
				glm::vec2 uv = temp_uvCoordinates[uvIndex - 1];
				out_uvCoordinates.push_back(uv);
				//std::cout << "uv succeslslf!\n";
			}
			for (unsigned int i = 0; i < normalIndices.size(); i++) {
				unsigned int normalIndex = normalIndices[i];
				glm::vec3 normal = temp_normals[normalIndex - 1];
				out_normals.push_back(normal);
				//std::cout << "normal successful!\n";
			}
		}

		std::unordered_map <std::string, Material> LoadMTLFile(const char* path, std::vector<std::string>& materialNames) {
			std::unordered_map<std::string, Material> temp_map;
			auto file = std::make_unique<std::ifstream>(path);
			if (!file) {
				throw std::runtime_error("Cannot open .mtl file!");
			}

			std::string line;
			Material currentMaterial;
			bool hasMaterial = false;

			while (std::getline(*file, line)) {
				std::istringstream iss(line);
				std::string token;

				iss >> token;
				if (token == "newmtl") {
					if (hasMaterial) temp_map[currentMaterial.name] = currentMaterial;

					currentMaterial = Material();
					iss >> currentMaterial.name;
					materialNames.push_back(currentMaterial.name);
					hasMaterial = true;
				}
				else if (token == "Ka") {
					iss >> currentMaterial.ambient.x >> currentMaterial.ambient.y >> currentMaterial.ambient.z;
				}
				else if (token == "Kd") {
					iss >> currentMaterial.diffusal.x >> currentMaterial.diffusal.y >> currentMaterial.diffusal.z;
				}
				else if (token == "Ks") {
					iss >> currentMaterial.specular.x >> currentMaterial.specular.y >> currentMaterial.specular.z;
				}
				else if (token == "Ns") {
					iss >> currentMaterial.shininess;
				}
				else if (token == "map_Kd") {
					iss >> currentMaterial.diffuseTexturePath;

					currentMaterial.hasTexture = true;
				}
			}
			if (hasMaterial) temp_map[currentMaterial.name] = currentMaterial;
			std::cout << "Successfully loaded .mtl file! \n";
			return temp_map;
		}

		std::unordered_map<std::string, Object> LoadScene(const char* path) {
			std::unordered_map<std::string, Object> objects;

			auto File = std::make_unique<std::ifstream>(path);

			if (!File) {
				throw std::exception("Impossible to open File");
			}

			std::string line;
			Object currentObject;
			bool hasObject = false;
			while (std::getline(*File, line)) {

				std::istringstream iss(line);
				std::string token;

				iss >> token;

				if (token == "newobj") {
					if (hasObject) {
						objects[currentObject.name] = currentObject;
					}

					currentObject = Object();
					iss >> currentObject.name;
					hasObject = true;
				}
				else if (token == "p") {
					iss >> currentObject.position.x >> currentObject.position.y >> currentObject.position.z;
				}
				else if (token == "r") {
					iss >> currentObject.rotation.x >> currentObject.rotation.y >> currentObject.rotation.z;
				}
				else if (token == "s") {
					iss >> currentObject.scale.x >> currentObject.scale.y >> currentObject.scale.z;
				}
				else if (token == "vSh") {
					iss >> currentObject.vertexShaderFile;
				}
				else if (token == "fSh") {
					iss >> currentObject.fragmentShaderFile;
				}
				else if (token == "mesh") {
					iss >> currentObject.meshFile;
				}
				else if (token == "p_mass") {
					iss >> currentObject.mass;
				}

				else if (token == "new_comp") {
					std::string compToken;
					iss >> compToken;

					if (compToken == "charController")
						currentObject.componentNames.push_back("Character Controller");
				}
			}
			if (hasObject)
				objects[currentObject.name] = currentObject;
			return objects;
		}

		OBJloader() = default;
	};

}