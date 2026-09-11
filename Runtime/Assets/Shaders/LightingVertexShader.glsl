#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextureCoords;
layout (location = 2) in vec3 aNormal;

out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoords;
out vec3 FragPos;

void main() {
	//only the global world position, not the view and projection
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(model))) * aNormal;
	texCoords = aTextureCoords;
	gl_Position = projection * view * vec4(FragPos, 1.0);
}