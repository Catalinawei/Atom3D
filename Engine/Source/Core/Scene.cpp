#include "Scene.h"

using namespace AtomToolKit;

Scene::Scene(ResourceManager* _rManager)
	: resourceManager(_rManager), lightId(1){
	m_actors = MemoryPool<Actor>();
}

void Scene::Initiallize(const char* scenePath, const char* skyboxVShader, const char* skyboxFShader) {
	m_physicsManager = std::make_unique<PhysicsManager>();

	resourceManager->LoadScene(scenePath);

	auto& SceneObjects = resourceManager->GetAllObjects();
	std::cout << SceneObjects.size() << "\n";

	for (auto& i : SceneObjects) {
		const std::string name = i.first;
		const auto& objectData = i.second;

		std::cout << name << "\n";

		GameObject* meshData = resourceManager->getObject(name.c_str());

		if (!meshData) {
			std::cout << "Failed to load object: " << name << "\n";
			continue;
		}

		std::cout << "SUCCESS: Loaded " << name
			<< " with " << meshData->data->vertices.size() << " vertices\n";

		Actor* actor = m_actors.Create();
		Sprite* spriteComp = new Sprite(*object_Shaders["LightingShader"], meshData->data->vertices,
			meshData->data->uv, meshData->data->normals);
		Transform* transCompo = new Transform(object_Shaders["LightingShader"].get());

		//spriteComp->enableTextures = false;
		transCompo->SetPosition(meshData->position);
		transCompo->SetScale(meshData->scale);
		transCompo->SetRotation(meshData->rotation);

		std::cout << meshData->rotation.x << " " << meshData->rotation.y
			<< " " << meshData->rotation.z << "\n";
		{
			btCollisionShape* shape = new btBoxShape(spriteComp->GetHalfExtents(meshData->vertices));
			btVector3 physicsTransformPos(transCompo->GetPosition().x, transCompo->GetPosition().y, 
			transCompo->GetPosition().z);

			glm::quat quat = glm::quat(glm::vec3(meshData->rotation));
			btQuaternion quaternion(quat.x, quat.y, quat.z, quat.w);
			PhysicsComponent* physicsComp = new PhysicsComponent(actor, m_physicsManager.get(), shape
				, physicsTransformPos, meshData->mass, quaternion);
			actor->AddComponent(physicsComp);
		}
		if (meshData->material)
			spriteComp->SetMaterial(*meshData->material);
		actor->AddComponent(transCompo);
		actor->AddComponent(spriteComp);
	}

	m_skyboxShader = std::make_unique<Shader>(skyboxVShader, skyboxFShader);
	m_defaultSkybox = std::make_unique<SkyBox>(m_skyboxShader.get());
	
	std::vector<std::string> skyboxTextures = {
		"Assets/Skybox/LearnOpengl/right.jpg",
		"Assets/Skybox/LearnOpengl/left.jpg",
		"Assets/Skybox/LearnOpengl/top.jpg",
		"Assets/Skybox/LearnOpengl/bottom.jpg",
		"Assets/Skybox/LearnOpengl/front.jpg",
		"Assets/Skybox/LearnOpengl/back.jpg"
	};

	std::cout << skyboxTextures.size() << "\n";

	m_defaultSkybox->Load(skyboxTextures);
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