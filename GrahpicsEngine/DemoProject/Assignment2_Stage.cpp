#include <filesystem>
#include <fstream>

#include "Assignment2_Stage.h"
#include "imgui.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Application.h"
#include "BoundingVolume.h"
#include "Camera.h"
#include "Graphic.h"
#include "Input.h"
#include "ModelLoader.h"
#include "ObjectManager.h"
#include "Object.h"
#include "Light.h"
#include "Mesh.h"
#include "ModelMesh.h"
#include "Transform.h"

Assignment2_Stage::Assignment2_Stage()
{
	activeAABB = true;
	activeRitterSphere = false;
	activeLarssonSphere = false;
	activePCAShpere = false;
}

void Assignment2_Stage::Initialize()
{
	InitializeGUI();

	CreateCamera();
	SetBaseLight();
	LoadPlantSection();
	CreateBVH();
}

void Assignment2_Stage::Update()
{
	float time = (float)glfwGetTime();
	float dt = time - lastUpdateTime;

	UpdateCamera(dt);
	lastUpdateTime = time;

	UpdateGUI();
}

void Assignment2_Stage::Close()
{

}

void Assignment2_Stage::InitializeGUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(APPLICATION->GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 430");
	ImGui::StyleColorsDark();
}

void Assignment2_Stage::UpdateGUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	UpdateDebugViewGUI();
	UpdateBV_Active();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Assignment2_Stage::UpdateBV_Active()
{
	ImGui::Begin("Bounding Volume Active Panel");

	ImGui::Checkbox("Active AABB", &activeAABB);
	ImGui::Checkbox("Active Ritter Sphere", &activeRitterSphere);
	ImGui::Checkbox("Active Larsson Sphere", &activeLarssonSphere);
	ImGui::Checkbox("Active PCA Sphere", &activePCAShpere);

	for (auto bv : bvLists)
	{
		bv->ActiveAABB = activeAABB;
		bv->ActiveRitterSphere = activeRitterSphere;
		bv->ActiveLarssonSphere = activeLarssonSphere;
		bv->ActivePCAShpere = activePCAShpere;
	}

	ImGui::End();
}

void Assignment2_Stage::UpdateDebugViewGUI()
{
	ImGui::Begin("Debug View Panel");

	GRAPHIC->DrawDeferredView();

	ImGui::End();
}

void Assignment2_Stage::LoadPlantSection()
{
	/* --------------- TEST START---------------*/
	/*MODELLOAD->Load("Test", "models/bunny_high_poly.obj", false);
	auto meshT = MODELLOAD->Get("Test");
	Object* object = new Object("Test");
	auto newMesh = new ModelMesh(*meshT);
	newMesh->material->AmbientColor = glm::vec3(1);
	newMesh->material->DiffuseColor = glm::vec3(1);
	newMesh->material->SpecularColor = glm::vec3(1);
	newMesh->UseDeferredRendering = true;
	object->mesh = newMesh;
	BoundingVolume* bv = new BoundingVolume(object);
	object->AddComponent(bv);
	bvLists.push_back(bv);
	OBJMANAGER->Add(object);*/
	/* --------------- TEST END ---------------*/

	std::fstream file("PowerPlants/" + sectionFileName);

	std::string str;
	ModelMesh* mesh;
	int i = 0;
	while (std::getline(file, str))
	{
		std::string modelPath = "PowerPlants/" + str;
		std::string objName = "PowerPlants_Model_" + std::to_string(i + 1);
		MODELLOAD->Load(objName, modelPath, true);
		mesh = MODELLOAD->Get(objName);

		Object* object = new Object(objName);
		auto newMesh = new ModelMesh(*mesh);
		newMesh->material->AmbientColor = glm::vec3(1);
		//newMesh->material->DiffuseColor = glm::vec3(0.8f);
		//newMesh->material->SpecularColor = glm::vec3(0.8f);
		newMesh->UseDeferredRendering = true;
		object->mesh = newMesh;

		BoundingVolume* bv = new BoundingVolume();
		bv->CreateByMesh(object);
		bv->ActiveAABB = activeAABB;
		bv->ActiveRitterSphere = activeRitterSphere;
		bv->ActiveLarssonSphere = activeLarssonSphere;
		bv->ActivePCAShpere = activePCAShpere;
		bvLists.push_back(bv);
		object->AddComponent(bv);

		OBJMANAGER->Add(object);
		i++;
	}
}

void Assignment2_Stage::SetBaseLight()
{
	Object* lightObject = new Object("Light_Base");

	Light* baseLight = new Light(lightObject);
	baseLight->Ambient = glm::vec3(1);
	//baseLight->Diffuse = glm::vec3(1);
	//baseLight->Specular = glm::vec3(1);
	baseLight->Type = LightType::Directional;
	baseLight->Direction = glm::vec3(0, 0, -1);
	lightObject->AddComponent(baseLight);

	OBJMANAGER->Add(lightObject);
}

void Assignment2_Stage::CreateCamera()
{
	Object* cameraObj = new Object("Main Camera");
	cameraObj->transform->Position = { 0,40000,80000 };

	Camera* camera = new Camera(cameraObj);
	camera->Far = 3000000;

	cameraObj->AddComponent(camera);
	OBJMANAGER->Add(cameraObj);
}

void Assignment2_Stage::UpdateCamera(float dt)
{
	auto cameraObject = GRAPHIC->MainCamera()->gameObject;
	glm::vec3 right = glm::vec3(1, 0, 0);
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 currentRotation = cameraObject->transform->Rotation;
	/*float moveSpeed = dt * 10.0f;
	float rotateSpeed = dt * 20.0f;*/
	float moveSpeed = dt * 100000.0f;
	float rotateSpeed = dt * 50.0f;

	if (INPUT->KeyPressed(GLFW_KEY_W))
		GRAPHIC->MainCamera()->Move(moveSpeed, CameraMoveAxis::Front);

	if (INPUT->KeyPressed(GLFW_KEY_S))
		GRAPHIC->MainCamera()->Move(moveSpeed, CameraMoveAxis::Back);

	if (INPUT->KeyPressed(GLFW_KEY_A))
		GRAPHIC->MainCamera()->Move(moveSpeed, CameraMoveAxis::Left);

	if (INPUT->KeyPressed(GLFW_KEY_D))
		GRAPHIC->MainCamera()->Move(moveSpeed, CameraMoveAxis::Right);

	if (INPUT->KeyPressed(GLFW_KEY_Q))
		cameraObject->transform->Rotation = currentRotation + (up * (rotateSpeed));

	if (INPUT->KeyPressed(GLFW_KEY_E))
		cameraObject->transform->Rotation = currentRotation + (-up * (rotateSpeed));

	if (INPUT->KeyPressed(GLFW_KEY_Z))
		cameraObject->transform->Rotation = currentRotation + (right * (rotateSpeed));

	if (INPUT->KeyPressed(GLFW_KEY_C))
		cameraObject->transform->Rotation = currentRotation + (-right * (rotateSpeed));
}

void Assignment2_Stage::CreateBVH()
{
	std::vector<BoundingVolume> bvTempList;

	for (auto bv : bvLists)
	{
		bvTempList.push_back(*bv);
	}

	bvh = new BoundingVolumeHierarchy(bvTempList);
	GRAPHIC->SetCurrentBVH(bvh);
}
