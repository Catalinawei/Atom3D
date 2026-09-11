#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include <iostream>
#include <vector>
#include "stb_image.h"

namespace AtomToolKit {

	class SkyBox {
	public:

		SkyBox(Shader* shader)
			: shader(shader), VAO(0), VBO(0), textureId(0) {

			vertices = {
				-1.0f,  1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				-1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f
			};
			std::cout << vertices.size() << "\n";
		}

		void Load(std::vector<std::string>& textures) {

			glGenBuffers(1, &VBO);
			glGenVertexArrays(1, &VAO);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			//tell opengl to write the vertices data to GL_ARRAY_BUFFER
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

			//set position of x,y,z in every stride
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0); //enable the data at the vertex shader

			//set textures of x,y in every stride
			//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Vertex::uv)));
			//glEnableVertexAttribArray(1); //enable the data at the vertex shader

			glBindVertexArray(0);



			glGenTextures(1, &textureId);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

			//just like eyes, stbi reads images flipped, so we just flipped it
			stbi_set_flip_vertically_on_load(false);

			int textureWidth, textureHeight, nrChannels;
			unsigned char* data;

			for (int i = 0; i < 6; i++) {
				std::cout << "Loading: " << textures[i] << "\n";

				data = stbi_load(textures[i].c_str(), &textureWidth, &textureHeight, &nrChannels, 0);

				std::cout << "Loaded: " << textures[i] << " -> " << textureWidth << "x" << textureHeight << ", " << nrChannels
					<< " channels\n";

				if (data) {
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, textureWidth, textureWidth, 0, GL_RGB, GL_UNSIGNED_BYTE,
						data);
					//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
					stbi_image_free(data);
				}
				else {
					std::cerr << "Failed to load texture\n";
				}

			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}

		void Render(const Camera& camera) {

			//since its the first to draw the skybox, we disable the depth test
			glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);

			shader->UseProgram();

			glActiveTexture(GL_TEXTURE0);

			shader->SetUniformInt("skybox", 0);
			shader->SetUniformMatrix4("cameraView", glm::mat4(glm::mat3(camera.GetViewMatrix())));
			shader->SetUniformMatrix4("projection", camera.GetProjectionMatrix());

			glBindVertexArray(VAO);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			//normal render for other game objects
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
		}
	private:
		unsigned int VAO, VBO;
		unsigned int textureId;
		Shader* shader;
		std::vector<float> vertices;
	};
}