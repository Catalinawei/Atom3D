#include "Sprite.h"
#include "Actor.h"
#include "Transform.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace AtomToolKit;

Sprite::Sprite(Shader& shader) : Component()
, VBO(0), VAO(0), m_shader(&shader), enableTextures(true), Texture(0), textureHeight(0), textureWidth(0),
hasMaterial(true), nrChannels(0)
{
}
Sprite::Sprite(Shader& shader, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& uvs,
	const std::vector<glm::vec3>& normals)
	: Component()
	, VBO(0), VAO(0), m_shader(&shader), enableTextures(true), hasMaterial(true),
	Texture(0), textureHeight(0), textureWidth(0), nrChannels(0), m_vertices(vertices) {

	for (size_t i = 0; i < vertices.size(); ++i) {
		dataSize.push_back(i);
	}

	if (!m_shader)
		std::cerr << "M_shader of sprite is nullptr\n";

	InitBuffers(vertices, uvs, normals);
}

void Sprite::InitBuffers(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs, std::vector<glm::vec3> normals) {

	std::vector<Vertex> result;
	for (size_t i = 0; i < vertices.size(); i++) {
		Vertex j;
		j.vertices = vertices[i];
		j.uv = uvs[i];
		j.normals = normals[i];
		result.push_back(j);
	}

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//tell opengl to write the vertices data to GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), result.data(), GL_STATIC_DRAW);

	//set position of x,y,z in every stride
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::vertices));
	glEnableVertexAttribArray(0); //enable the data at the vertex shader

	if (enableTextures) {
		//set textures of x,y in every stride
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Vertex::uv)));
		glEnableVertexAttribArray(1); //enable the data at the vertex shader
	}

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Vertex::normals)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Sprite::GenerateTextures(const char* texturePath) {
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	//texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//just like eyes, stbi reads images flipped, so we just flipped it
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(texturePath, &textureWidth, &textureHeight, &nrChannels, 0);
	GLenum im_format;
	if (nrChannels == 1)
		im_format = GL_RED;
	else if (nrChannels == 3)
		im_format = GL_RGB;
	else if (nrChannels == 4)
		im_format = GL_RGBA;
	else
		im_format = GL_RGB;

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, im_format, textureWidth, textureHeight, 0, im_format, GL_UNSIGNED_BYTE,
			data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cerr << "Failed to load texture\n";
	}
	m_material.diffuseTextureId = Texture;
	stbi_image_free(data);
}

void Sprite::Update(float deltaTime) {

}

void Sprite::Render() {
	if (VAO == 0 || dataSize.empty()) {
		std::cerr << "Sprite::Render() - VAO or dataSize is empty\n";
		return;
	}

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)dataSize.size());
	glBindVertexArray(0);
}

Sprite::~Sprite() {
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VAO);
}

btVector3 Sprite::GetHalfExtents(std::vector<glm::vec3>& vertices, const glm::vec3& scale) {
	btVector3 minVec(FLT_MAX, FLT_MAX, FLT_MAX);
	btVector3 maxVec(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (auto i : vertices) {
		minVec.setX(std::min(minVec.x(), i.x));
		minVec.setY(std::min(minVec.y(), i.y));
		minVec.setZ(std::min(minVec.z(), i.z));
		maxVec.setX(std::max(maxVec.x(), i.x));
		maxVec.setY(std::max(maxVec.y(), i.y));
		maxVec.setZ(std::max(maxVec.z(), i.z));
	}

	btVector3 center = (maxVec + minVec) * 0.5f;
	btVector3 halfExtents = (maxVec - minVec) * 0.5f;

	halfExtents.setX(halfExtents.x() * scale.x);
	halfExtents.setY(halfExtents.y() * scale.y);
	halfExtents.setZ(halfExtents.z() * scale.z);

	std::cout << "CHARACTER BOUNDS\n";
	std::cout << "minY: " << minVec.y() << "\n";
	std::cout << "maxY: " << maxVec.y() << "\n";

	center.setX(center.x() * scale.x);
	center.setY(center.y() * scale.y);
	center.setZ(center.z() * scale.z);

	std::cout << "BOUNDS\n";
	std::cout << "minY: " << minVec.y() << "\n";
	std::cout << "maxY: " << maxVec.y() << "\n";
	std::cout << "centerY: " << center.y() << "\n";
	std::cout << "halfY: " << halfExtents.y() << "\n";


	return halfExtents;
}

btVector3 Sprite::GetCenter(std::vector<glm::vec3>& vertices, const glm::vec3& scale) {
	btVector3 minVec(FLT_MAX, FLT_MAX, FLT_MAX);
	btVector3 maxVec(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (auto i : vertices) {
		minVec.setX(std::min(minVec.x(), i.x));
		minVec.setY(std::min(minVec.y(), i.y));
		minVec.setZ(std::min(minVec.z(), i.z));
		maxVec.setX(std::max(maxVec.x(), i.x));
		maxVec.setY(std::max(maxVec.y(), i.y));
		maxVec.setZ(std::max(maxVec.z(), i.z));
	}

	btVector3 center = (maxVec + minVec) * 0.5f;

	center.setX(center.x() * scale.x);
	center.setY(center.y() * scale.y);
	center.setZ(center.z() * scale.z);

	return center;
}