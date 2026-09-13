#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Component.h"
#include "PhysicsComponent.h"
#include "InputManager.h"
#include "Camera.h"
#include "Sprite.h"

namespace AtomToolKit {
	class CharacterController : public AtomToolKit::Component {
	public:
		CharacterController() :
			m_physics(nullptr), m_camera(nullptr), m_window(nullptr), moveSpeed(10.0f), m_friction(1.0f), jumpForce(2500.0f), jumpingToggle(false), m_owner(nullptr),
			groundCheckDistance(1.0f), height(1.8f), maxSpeed(1000.0f){}

		void Setup(AtomToolKit::Actor* actor, AtomToolKit::PhysicsComponent* physics, AtomToolKit::Camera* camera, GLFWwindow* window, float speed);
		void Update(float deltaTime) override;

	public:
		void SetFriction(const btScalar& friction) {
			m_friction = friction;
		}
		void SetMaxSpeed(const btScalar& speed) {
			maxSpeed = speed;
		}

		bool grounded() {
			btVector3 start = m_physics->GetRigidBody()->getWorldTransform().getOrigin() - btVector3(0, height, 0);
			btVector3 end = start - btVector3(0, -groundCheckDistance, 0);
			
			btCollisionWorld::ClosestRayResultCallback callback(start, end);

			m_physics->GetManager()->getWorld()->rayTest(start, end, callback);

			//std::cout << callback.hasHit() << "\n";

			return callback.hasHit();
		}

	private:
		glm::vec3 cameraForward = glm::vec3(0.0f);
		glm::vec3 cameraCross = glm::vec3(0.0f);
		btVector3 forward = btVector3(0.f, 0.f, 0.f);
		btVector3 right = btVector3(0.f, 0.f, 0.f);
		btVector3 up = btVector3(0.f, 0.f, 0.f);
	private:

		AtomToolKit::PhysicsComponent* m_physics;
		AtomToolKit::Camera* m_camera;
		AtomToolKit::Actor* m_owner;
		//AtomToolKit::Sprite* m_sprite;
		GLFWwindow* m_window;

		float m_friction;
		float moveSpeed;
		float jumpForce;
		bool jumpingToggle;
		float groundCheckDistance;
		float height;

		float maxSpeed;
	};
}