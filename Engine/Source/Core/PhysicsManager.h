#pragma once
#include "btBulletDynamicsCommon.h"
#include <memory>
#include <vector>
#include <iostream>

namespace AtomToolKit {
	class PhysicsManager
	{
	public:
		PhysicsManager();
		~PhysicsManager();
		void Update(float deltaTime);

		void AddCollisionShape(btCollisionShape* shape) {
			collisionShapes.push_back(shape);
			std::cout << "Added collision shape!\n";
		}

		void AddRigidBody(btRigidBody* body) {
			rigidBodies.push_back(body);
			physicsWorld->addRigidBody(body);
			std::cout << "Added rigidboy\n";
		}

		void AddMotionState(btDefaultMotionState* state) {
			motions.push_back(state);
			std::cout << "Added State\n";
		}

		btDynamicsWorld* getWorld() const noexcept {
			return physicsWorld.get();
		}

	public:
		btBroadphaseInterface* getOverlappingCache() const noexcept {
			return overlappingPairCache.get();
		}

	private:
		//bullet configuration and all
		std::unique_ptr<btDefaultCollisionConfiguration> configuration;
		std::unique_ptr<btCollisionDispatcher> dispatcher;
		std::unique_ptr<btBroadphaseInterface> overlappingPairCache;
		std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
		std::unique_ptr<btDynamicsWorld> physicsWorld;

		//bullet likes raw pointers 
		std::vector<btCollisionShape*> collisionShapes;
		std::vector<btRigidBody*> rigidBodies;
		std::vector<btDefaultMotionState*> motions;
	};

}