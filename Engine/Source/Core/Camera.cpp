#include "Camera.h"

using namespace AtomToolKit;

Camera::Camera(Shader* shader, GLFWwindow* window) : m_shader(shader), view(glm::mat4(1.0f)), projection(glm::mat4(1.0f)),
m_window(window){
	cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    firstMouse = true;
    yaw = -90.0f;
    pitch = 0.0f;
    FOV = 90.0f;
	lastX = 0.0f; lastY = 0.0f;
}

void Camera::Update(float deltaTime) {
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	m_shader->SetUniformMatrix4("view", view);

	//camera and projection things
	{
		int viewportWidth, viewportHeight;
		glfwGetFramebufferSize(m_window, &viewportWidth, &viewportHeight);

		//cant divide by 0!
		if (viewportHeight == 0) viewportHeight = 1;
		if (viewportWidth == 0) viewportWidth = 1;

		projection = glm::perspective(glm::radians(FOV) /*fov*/, (float)viewportWidth / (float)viewportHeight /*proportion*/,
			0.1f /*near plane*/, 100.0f /*far plane*/);
	}
}

void Camera::ProcessInput(GLFWwindow* window, float deltaTime) {

	float cameraSpeed = 7.0f;

	glm::vec3 front = cameraSpeed * cameraFront;
	glm::vec3 right = cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));

	if (InputManager::KeyPressed(m_window, GLFW_KEY_W)) {
		cameraPos += front * deltaTime;
	}
	if(InputManager::KeyPressed(m_window, GLFW_KEY_S)){
		cameraPos -= front * deltaTime;
	}

	if(InputManager::KeyPressed(m_window, GLFW_KEY_A)){
		cameraPos -= right * deltaTime;
	}
	if (InputManager::KeyPressed(window, GLFW_KEY_D)) {
		cameraPos += right * deltaTime;
	}
}

void Camera::Look(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}