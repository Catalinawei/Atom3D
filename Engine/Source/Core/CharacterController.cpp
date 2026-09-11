#include "CharacterController.h"

using namespace AtomToolKit;

void CharacterController::Setup(Actor* actor, PhysicsComponent* physics, Camera* camera, GLFWwindow* window, float speed) {
	m_physics = physics;
	m_camera = camera;
	moveSpeed = speed; 
	m_window = window;
	m_owner = actor;

	if (!m_physics || !m_camera) {
		std::cerr << m_physics ? "CharacterController requires a valid PhysicsComponent\n" : "CharacterController requires a valid Camera\n";
	}
}

void CharacterController::Update(float deltaTime) {
	auto cameraForward = m_camera->GetCameraFront() * moveSpeed;
	auto cameraCross   = moveSpeed * glm::normalize(glm::cross(m_camera->GetCameraFront(), m_camera->GetCameraUp()));
	btVector3 forward = btVector3(cameraForward.x, cameraForward.y, cameraForward.z);
	btVector3 right   = btVector3(cameraCross.x, cameraCross.y, cameraCross.z);
	btVector3 up      = btVector3(0.f, jumpForce, 0.f);
	
	if (glfwGetKey(m_window, GLFW_KEY_W)) {
		m_physics->GetRigidBody()->activate(true);
		m_physics->GetRigidBody()->applyCentralForce(forward);
	}
	if (glfwGetKey(m_window, GLFW_KEY_S)) {
		m_physics->GetRigidBody()->activate(true);
		m_physics->GetRigidBody()->applyCentralForce(-forward);
	}
	if (glfwGetKey(m_window, GLFW_KEY_A)) {
		m_physics->GetRigidBody()->activate(true);
		m_physics->GetRigidBody()->applyCentralForce(-right);
	}
	if (glfwGetKey(m_window, GLFW_KEY_D)) {
		m_physics->GetRigidBody()->activate(true);
		m_physics->GetRigidBody()->applyCentralForce(right);
	}
	if(glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS&& grounded()){
		m_physics->GetRigidBody()->activate(true);
		m_physics->GetRigidBody()->applyCentralForce(up);
	}

	//m_owner->GetComponent<Transform>()->SetRotation(glm::quat(glm::vec3(0, glm::radians(-forward), 0)));
}