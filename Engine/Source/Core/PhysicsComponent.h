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
			const btVector3 physicsPosition, float mass, const btQuaternion& initialQuaternion)
			: Component(), m_mass(mass), m_rigidBody(nullptr), manager(p_manager), m_owner(owner) {

			std::cout << "Initiallizing physics component\n";

			if (!manager)
				std::cerr << "Cannot access PhysicsManager\n";
			if (!m_owner)
				std::cerr << "CANNOT ACCESS ACTOR\n";

			//assume we have only basic meshes
			manager->AddCollisionShape(objectShape);

			btTransform transform;
			transform.setIdentity();
			//set the transform position
			transform.setOrigin(physicsPosition);
			transform.setRotation(initialQuaternion);

			//if mass is 0. then...
			bool isDynamic = (mass != 0.f);

			//localInertia = how hard to spin
			btVector3 localInertia(0, 0, 0);
			if (isDynamic)
				objectShape->calculateLocalInertia(mass, localInertia);

			//Motion state allows for interpolation and syncrnization for active objects
			btDefaultMotionState* motionState = new btDefaultMotionState(transform);
			//just setting the properties of our rigidbody
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, objectShape, localInertia);
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

	private:
		PhysicsManager* manager;
		btRigidBody* m_rigidBody;
		float m_mass;

		Actor* m_owner;
	};
}