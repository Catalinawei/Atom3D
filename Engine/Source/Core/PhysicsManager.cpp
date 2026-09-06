#include "PhysicsManager.h"

using namespace AtomToolKit;

PhysicsManager::PhysicsManager() {
	//initiallize all the physics things
	configuration = std::make_unique<btDefaultCollisionConfiguration>();
	dispatcher = std::make_unique<btCollisionDispatcher>(configuration.get());
	overlappingPairCache = std::make_unique<btDbvtBroadphase>();
	solver = std::make_unique<btSequentialImpulseConstraintSolver>();
	physicsWorld = std::make_unique<btDiscreteDynamicsWorld>(dispatcher.get()
		, overlappingPairCache.get(), solver.get(), configuration.get());

	physicsWorld->setGravity(btVector3(0, -10, 0));
}

void PhysicsManager::Update(float deltaTime) {
	physicsWorld->stepSimulation(deltaTime, 10);
}

PhysicsManager::~PhysicsManager() {
	//delete collision shapes
	for (auto shape : collisionShapes) {
		delete shape;
	}

	for (auto body : rigidBodies) {
		physicsWorld->removeRigidBody(body);
		delete body;
	}
	
	for (auto state : motions) {
		delete state;
	}

	//smart pointers automatically cleared
}