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
	std::cout << "JumpForce of character Controller " << up.y() << "\n";
}

void CharacterController::Update(float deltaTime) {
	cameraForward = m_camera->GetCameraFront() * moveSpeed * m_friction;
	cameraCross   = moveSpeed * glm::normalize(glm::cross(m_camera->GetCameraFront(), m_camera->GetCameraUp()));
	forward = btVector3(cameraForward.x, cameraForward.y, cameraForward.z);
	right   = btVector3(cameraCross.x, cameraCross.y, cameraCross.z);
	up		= btVector3(0.f, jumpForce, 0.f);
	
	if (glfwGetKey(m_window, GLFW_KEY_W)) {
		m_physics->GetRigidBody()->activate(true);
		m_physics->GetRigidBody()->applyCentralForce(forward );
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
	if(glfwGetKey(m_window,  GLFW_KEY_SPACE) == GLFW_PRESS && grounded()){
		m_physics->GetRigidBody()->activate(true);
		m_physics->GetRigidBody()->applyCentralForce(up);
	}

	//if(m_physics->GetRigidBody()->getAngularVelocity().x() > maxSpeed)
	//	m_physics->GetRigidBody()->setAngularVelocity(btVector3(maxSpeed, 0.f, 0.f));
	//if (m_physics->GetRigidBody()->getAngularVelocity().z() > maxSpeed)
	//	m_physics->GetRigidBody()->setAngularVelocity(btVector3(0.f, 0.f, maxSpeed));
   
	btTransform transform = m_physics->GetRigidBody()->getWorldTransform();
	btVector3 pos		  = transform.getOrigin();
	m_camera->SetCameraPos(glm::vec3(pos.x(), pos.y() + 5.0f, pos.z() - 3.f));
}