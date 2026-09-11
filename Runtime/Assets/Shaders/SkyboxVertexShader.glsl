#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 cameraView;
uniform mat4 projection;

out vec3 texCoords;

void main() {
	texCoords = aPos;

	mat4 viewNoTranslation = cameraView;
	gl_Position = projection * viewNoTranslation * vec4(aPos, 1.0);
}