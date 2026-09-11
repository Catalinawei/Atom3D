#pragma once
#include "Component.h"
#include "PhysicsManager.h"
#include "Transform.h"

using namespace AtomToolKit;

namespace AtomToolKit {
	class PhysicsComponent
		: public Component {
	public:
		PhysicsComponent(Actor* owner, PhysicsManager* p_manager, btCollisionShape* objectShape,
			const btVector3 physicsPosition, float mass, const btQuaternion& initialQuaternion, const btVector3& center)
			: Component(), m_mass(mass), m_rigidBody(nullptr), manager(p_manager), m_owner(owner) {

			std::cout << "Initiallizing physics component\n";

			if (!manager)
				std::cerr << "Cannot access PhysicsManager\n";
			if (!m_owner)
				std::cerr << "CANNOT ACCESS ACTOR\n";

			btTransform transform;
			transform.setIdentity();
			//set the transform position
			transform.setOrigin(physicsPosition);
			transform.setRotation(initialQuaternion);

			//if mass is 0. then the object is static 
			bool isDynamic = (mass != 0.f);

			//it works
			btTransform childTransform;
			childTransform.setIdentity();
			childTransform.setOrigin(center);

			btCompoundShape* compound = new btCompoundShape();
			compound->addChildShape(childTransform, objectShape);

			manager->AddCollisionShape(compound);

			//localInertia = how hard to spin
			btVector3 localInertia(0, 0, 0);
			//if its static, then it doesnt need to spin
			if (isDynamic)
				compound->calculateLocalInertia(mass, localInertia);

			//Motion state allows for interpolation and syncrnization for active objects
			btDefaultMotionState* motionState = new btDefaultMotionState(transform);
			//just setting the properties of our rigidbody
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, compound, localInertia);
			m_rigidBody = new btRigidBody(rbInfo);

			//passing to the manager
			manager->AddMotionState(motionState);
			manager->AddRigidBody(m_rigidBody);

			std::cout << "correctly initiallized physics component\n";
		}

		void Update(float deltaTime) override {

			if (m_rigidBody && m_rigidBody->getMotionState()) {

				btTransform btTransform;

				m_rigidBody->getMotionState()->getWorldTransform(btTransform);

				Transform* transformComponent = m_owner->GetComponent<Transform>();
				if (transformComponent) {
					btVector3 pos = btTransform.getOrigin();
					btQuaternion rot = btTransform.getRotation();
					transformComponent->SetPosition(glm::vec3(pos.getX(), pos.getY(), pos.getZ()));
					transformComponent->SetRotation(glm::quat(rot.w(), rot.x(), rot.y(), rot.z()));
				}
			}
		}

	public:
		btRigidBody* GetRigidBody() const noexcept {
			return m_rigidBody;
		}

		PhysicsManager* GetManager() const noexcept {
			return manager;
		}

	private:
		PhysicsManager* manager;
		btRigidBody* m_rigidBody;
		float m_mass;

		Actor* m_owner;
	};
}