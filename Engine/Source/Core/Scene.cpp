#include "Scene.h"

using namespace AtomToolKit;

Scene::Scene(ResourceManager* _rManager)
	: resourceManager(_rManager), lightId(1){
	m_actors = MemoryPool<Actor>();
}

void Scene::Initiallize(const char* scenePath, Camera* camera, GLFWwindow* window) {
	m_physicsManager = std::make_unique<PhysicsManager>();

	resourceManager->LoadScene(scenePath);

	auto& SceneObjects = resourceManager->GetAllObjects();

	for (auto& i : SceneObjects) {
		const std::string name = i.first;
		const auto& objectData = i.second;

		GameObject* meshData = resourceManager->getObject(name.c_str());

		if (!meshData) {
			std::cout << "Failed to load object: " << name << "\n";
			continue;
		}

		Actor* actor = m_actors.Create();

		{
			Sprite* spriteComp = new Sprite(*object_Shaders["LightingShader"], meshData->data->vertices,
				meshData->data->uv, meshData->data->normals);
			Transform* transCompo = new Transform(object_Shaders["LightingShader"].get());

			actor->SetName(name.c_str());

			//spriteComp->enableTextures = false;
			transCompo->SetPosition(meshData->position);
			transCompo->SetScale(meshData->scale);
			transCompo->SetRotation(meshData->rotation);
			{
				btCollisionShape* shape = new btBoxShape(spriteComp->GetHalfExtents(meshData->vertices, meshData->scale));
				//make tiny thing around the shape
				shape->setMargin(0.0f);

				btVector3 physicsTransformPos(transCompo->GetPosition().x, transCompo->GetPosition().y,
					transCompo->GetPosition().z);

				glm::quat quat = glm::quat(glm::vec3(meshData->rotation));
				btQuaternion quaternion(quat.x, quat.y, quat.z, quat.w);
				PhysicsComponent* physicsComp = new PhysicsComponent(actor, m_physicsManager.get(), shape
					, physicsTransformPos, meshData->mass, quaternion, spriteComp->GetCenter(meshData->vertices, meshData->scale));

				actor->AddComponent(physicsComp);

				auto& modifiableData = *meshData;
				for (auto& i : objectData.componentNames) {					
					if (i == "Character Controller") {
						std::cout << "CharacterController found\n";
						CharacterController* controller = new CharacterController();
						controller->Setup(actor, physicsComp, camera, window, 500.0f);
						controller->SetMaxSpeed(1000.f);
						controller->SetFriction(1.f);
						modifiableData.components.push_back(controller);
						actor->AddComponent<CharacterController>(controller);
					}
				}
			}
			if (meshData->material)
				spriteComp->SetMaterial(*meshData->material);
			actor->AddComponent(transCompo);
			actor->AddComponent(spriteComp);

		}
		
	}
}

void Scene::Render() {}

void Scene::Update(float deltaTime) {
	m_physicsManager->Update(deltaTime);
	for (int i = 0; i < m_actors.GetPoolSize(); ++i) {
		if(m_actors.isValid(i)){
			Actor* object = m_actors.GetObject(i);
			object->Update(deltaTime);
		}
	}
}

std::vector<Actor*> Scene::GetActors() {
	std::vector<Actor*> temp;
	for (size_t i = 0; i < m_actors.GetPoolSize(); ++i) {
		if (m_actors.isValid(i)) {
			auto j = m_actors.GetObject(i);
			temp.push_back(j);
		}
	}
	//std::cout << temp.size() << "\n";
	return temp;
}