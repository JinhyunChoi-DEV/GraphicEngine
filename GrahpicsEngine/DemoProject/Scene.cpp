#include <filesystem>

#include "Scene.h"

#include "Camera.h"
#include "Input.h"
#include "Light.h"
#include "ModelMesh.h"
#include "ObjectManager.h"
#include "ModelLoader.h"
#include "OrbitLineCreator.h"
#include "QuadCreator.h"
#include "ShaderManager.h"
#include "SphereCreator.h"
#include "TextureLoader.h"
#include "Transform.h"

void Scene::Initialize()
{
	TEXTURES->Load("Diff", "metal_roof_diff_512x512.png");
	TEXTURES->Load("Spec", "metal_roof_spec_512x512.png");
	LoadAllModel();
	rotationTime = 0.0f;
	lastUpdateTime = 0.0f;
	activeLightCount = 5;

	mainObject = new Object("Main Object");
	auto mesh = new ModelMesh(*MODELLOAD->Get("bunny_high_poly"));
	//mesh->DrawFaceNormal = true;
	//mesh->DrawVertexNormal = true;
	mesh->UseDeferredRendering = true;
	mesh->material->UseGpuUV = true;
	mesh->material->AmbientColor = glm::vec3(0);
	mesh->material->DiffuseColor = glm::vec3(1);
	mesh->material->SpecularColor = glm::vec3(1);
	mesh->material->DiffuseTexture = TEXTURES->Get("Diff");
	mesh->shader = SHADERS->Get("Phong");
	mainObject->mesh = mesh;
	OBJMANAGER->Add(mainObject);

	auto floor = new Object("Floor");
	floor->mesh = CreateQuad();
	floor->mesh->UseDeferredRendering = true;
	floor->mesh->material->AmbientColor = glm::vec3(0);
	floor->mesh->material->DiffuseColor = glm::vec3(0.8f);
	floor->mesh->material->SpecularColor = glm::vec3(0.8f);
	floor->mesh->shader = SHADERS->Get("Phong");
	floor->transform->Position = glm::vec3(0, -1.5, 0);
	floor->transform->Scale = glm::vec3(10);
	floor->transform->Rotation = glm::vec3(-90, 0, 0);
	OBJMANAGER->Add(floor);

	CreateLight();
	CreateOrbit();

	Object* obj = new Object("Main Camera");
	Camera* camera = new Camera(obj);
	obj->AddComponent(camera);
	OBJMANAGER->Add(obj);
	obj->transform->Position = { 0, 1, 10 };
}

void Scene::Update()
{
	float time = (float)glfwGetTime();
	float dt = time - lastUpdateTime;

	UpdateLights(dt);

	lastUpdateTime = time;
}

void Scene::Close()
{
}

void Scene::LoadAllModel()
{
	std::string path = MODELLOAD->rootPath;
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		auto filePath = entry.path().filename();
		auto fileFull = filePath.string();
		auto lastIndex = fileFull.find_last_of(".");
		auto file = fileFull.substr(0, lastIndex);
		modelNames.push_back(file);
		MODELLOAD->Load(file, fileFull);
	}
}

void Scene::CreateOrbit()
{
	Mesh* orbitLineMesh = CreateOrbitLine(1, 50);
	orbitLineMesh->UseDeferredRendering = true;
	orbitLineMesh->material->AmbientColor = glm::vec3(1);
	orbitLineMesh->material->DiffuseColor = glm::vec3(1);
	orbitLineMesh->material->SpecularColor = glm::vec3(1);

	Object* obj = new Object("orbit");
	obj->transform->Scale = glm::vec3(5, 5, 5);
	obj->mesh = orbitLineMesh;
	obj->mesh->UseDeferredRendering = true;
	OBJMANAGER->Add(obj);
}

void Scene::CreateLight()
{
	int count = 16;
	std::string baseName = "Light";
	constexpr float pi = glm::pi<float>();
	float step = 2 * pi / (float)count;
	Mesh* sphereMesh = CreateSphere(1.0f, 25, 25);
	std::string name = "";

	for (int i = 0; i < count; ++i)
	{
		float angle = step * (float)i;
		float x = cosf(angle);
		float y = 0.0f;
		float z = sinf(angle);

		name = baseName + std::to_string(i + 1);

		Object* obj = new Object(name);
		obj->mesh = sphereMesh;
		obj->transform->Position = glm::vec3(x * 5, y, z * 5);
		obj->transform->Scale = glm::vec3(0.2, 0.2, 0.2);
		if (activeLightCount <= i)
			obj->IsActive = false;

		Light* light = new Light(obj);
		light->Type = LightType::Point;
		light->Direction = glm::vec3(1, 0, 0);
		light->InnerAngle = 23.0f;
		light->OuterAngle = 27.5f;
		lights.push_back(obj);
		obj->AddComponent(light);

		OBJMANAGER->Add(obj);
	}

	lightOriginPosition = lights[0]->transform->Position;
}

void Scene::UpdateLights(float dt)
{
	auto targetPosition = lights[0]->transform->Position;
	if (glm::distance(lightOriginPosition, targetPosition) <= 0.1 && needResetRotationTimer)
	{
		rotationTime = 0.0f;
		needResetRotationTimer = false;
	}
	else if (glm::distance(lightOriginPosition, targetPosition) <= 0.1 && !needResetRotationTimer)
	{
		rotationTime += dt;
	}
	else
	{
		needResetRotationTimer = true;
		rotationTime += dt;
	}

	constexpr float pi = glm::pi<float>();
	float step = 2.0f * pi / activeLightCount;
	for (int i = 0; i < activeLightCount; ++i)
	{
		float additionAngle = step * i;
		float x = 5.0f * cosf(rotationTime + additionAngle);
		float y = 0;
		float z = 5.0f * sinf(rotationTime + additionAngle);
		lights[i]->transform->Position = glm::vec3(x, y, z);

		auto light = lights[i]->GetComponent<Light>();

		auto newDir = mainObject->transform->Position - lights[i]->transform->Position;
		light->Direction = newDir;
	}
}
