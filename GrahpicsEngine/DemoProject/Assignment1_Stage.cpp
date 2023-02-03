/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Assignment1_Stage.cpp
Purpose: Making of implementing code to setup assignment1 scene
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#include <filesystem>
#include "imgui.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "Assignment1_Stage.h"

#include "Application.h"
#include "Camera.h"
#include "Graphic.h"
#include "imgui_internal.h"
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

void Assignment1_Stage::Initialize()
{
	InitializeGUI();
	LoadAllModel();
	TEXTURES->Load("Diff", "metal_roof_diff_512x512.png");
	TEXTURES->Load("Spec", "metal_roof_spec_512x512.png");
	TEXTURES->Load("TestUV", "grid.png");

	rotationTime = 0.0f;
	lastUpdateTime = 0.0f;
	activeLightCount = 1;
	activeTestUV = false;
	activeRotation = true;

	CreateCamera();
	CreateDeferredObject();
	CreateLight();
	CreateOrbit();
}

void Assignment1_Stage::Update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	float time = (float)glfwGetTime();
	float dt = time - lastUpdateTime;

	if (activeRotation)
		RotateLights(dt);

	mainObject->transform->Rotation += glm::vec3(0, dt * 10, 0);
	UpdateCamera(dt);
	UpdateGUI();

	lastUpdateTime = time;

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Assignment1_Stage::Close()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Assignment1_Stage::InitializeGUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(APPLICATION->GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 430");
	ImGui::StyleColorsDark();
}

void Assignment1_Stage::LoadAllModel()
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

void Assignment1_Stage::CreateCamera()
{
	Object* obj = new Object("Main Camera");
	Camera* camera = new Camera(obj);
	obj->AddComponent(camera);
	OBJMANAGER->Add(obj);
	obj->transform->Position = { 0,2.5f,15 };
	camera->Position = obj->transform->Position;
}

void Assignment1_Stage::CreateDeferredObject()
{
	ModelMesh* modelMesh;
	if (MODELLOAD->Get("bunny_high_poly") != nullptr)
		modelMesh = MODELLOAD->Get("bunny_high_poly");
	else
		modelMesh = MODELLOAD->Get(modelNames[0]);

	selectedModelName = modelMesh->Name;
	mainObject = new Object("Main Object");
	auto mesh = new ModelMesh(*modelMesh);
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
	floor->transform->Position = glm::vec3(0, -1, 0);
	floor->transform->Scale = glm::vec3(10);
	floor->transform->Rotation = glm::vec3(-90, 0, 0);
	OBJMANAGER->Add(floor);
}

void Assignment1_Stage::CreateOrbit()
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

void Assignment1_Stage::CreateLight()
{
	int count = 16;
	std::string baseName = "Light";
	constexpr float pi = glm::pi<float>();
	float step = 2 * pi / (float)count;
	StaticMesh* sphereMesh = CreateSphere(1.0f, 25, 25);
	std::string name = "";
	auto shader = SHADERS->Get("Default");

	for (int i = 0; i < count; ++i)
	{
		float angle = step * (float)i;
		float x = cosf(angle);
		float y = 0.0f;
		float z = sinf(angle);

		name = baseName + std::to_string(i + 1);

		Object* obj = new Object(name);
		obj->mesh = new StaticMesh(*sphereMesh);
		obj->mesh->shader = shader;
		obj->mesh->material->AmbientColor = glm::vec3(1);
		obj->mesh->material->DiffuseColor = glm::vec3(1);
		obj->mesh->material->SpecularColor = glm::vec3(1);
		obj->transform->Position = glm::vec3(x * 5, y, z * 5);
		obj->transform->Scale = glm::vec3(0.2, 0.2, 0.2);
		if (activeLightCount <= i)
			obj->IsActive = false;

		Light* light = new Light(obj);
		light->Type = LightType::Point;
		light->Direction = glm::vec3(1, 0, 0);
		light->InnerAngle = 23.0f;
		light->OuterAngle = 27.5f;
		obj->AddComponent(light);
		lights.push_back(obj);

		OBJMANAGER->Add(obj);
	}

	lightOriginPosition = lights[0]->transform->Position;
	selectedLightIndex = 0;
}

void Assignment1_Stage::RotateLights(float dt)
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

	UpdateLights();
}

void Assignment1_Stage::UpdateCamera(float dt)
{
	auto cameraObject = GRAPHIC->MainCamera()->gameObject;
	glm::vec3 right = glm::vec3(1, 0, 0);
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 currentRotation = cameraObject->transform->Rotation;
	float speed = dt * 10.0f;

	glm::vec3 axis(0);

	if (INPUT->KeyPressed(GLFW_KEY_W))
		GRAPHIC->MainCamera()->Move(speed, CameraMoveAxis::Front);

	if (INPUT->KeyPressed(GLFW_KEY_S))
		GRAPHIC->MainCamera()->Move(speed, CameraMoveAxis::Back);

	if (INPUT->KeyPressed(GLFW_KEY_A))
		GRAPHIC->MainCamera()->Move(speed, CameraMoveAxis::Left);

	if (INPUT->KeyPressed(GLFW_KEY_D))
		GRAPHIC->MainCamera()->Move(speed, CameraMoveAxis::Right);

	if (INPUT->KeyPressed(GLFW_KEY_Q))
		cameraObject->transform->Rotation = currentRotation + (up * (speed * 2));

	if (INPUT->KeyPressed(GLFW_KEY_E))
		cameraObject->transform->Rotation = currentRotation + (-up * (speed * 2));

	if (INPUT->KeyPressed(GLFW_KEY_Z))
		cameraObject->transform->Rotation = currentRotation + (right * (speed * 2));

	if (INPUT->KeyPressed(GLFW_KEY_C))
		cameraObject->transform->Rotation = currentRotation + (-right * (speed * 2));
}

void Assignment1_Stage::UpdateLights()
{
	constexpr float pi = glm::pi<float>();
	float step = 2.0f * pi / activeLightCount;
	for (int i = 0; i < lights.size(); ++i)
	{
		if (i < activeLightCount)
		{
			float additionAngle = step * i;
			float x = 5.0f * cosf(rotationTime + additionAngle);
			float y = 0;
			float z = 5.0f * sinf(rotationTime + additionAngle);
			lights[i]->transform->Position = glm::vec3(x, y, z);

			auto light = lights[i]->GetComponent<Light>();

			auto newDir = mainObject->transform->Position - lights[i]->transform->Position;
			light->Direction = newDir;
			lights[i]->IsActive = true;
		}
		else
		{
			lights[i]->IsActive = false;
		}
	}
}

void Assignment1_Stage::UpdateGUI()
{
	UpdateModelGUI();
	UpdateLightGUI();
	UpdateDeferredRenderGUI();
	UpdateDebugViewGUI();
}

void Assignment1_Stage::UpdateModelGUI()
{
	ImGui::Begin("Object Panel");
	ImGui::Text("Models");
	if (ImGui::BeginCombo("Change Model", selectedModelName.c_str()))
	{
		for (unsigned int i = 0; i < modelNames.size(); ++i)
		{
			bool isSelected = selectedModelName == modelNames[i];
			if (ImGui::Selectable(modelNames[i].c_str(), isSelected))
			{
				mainObject->mesh->CopyData(*MODELLOAD->Get(modelNames[i]));
				selectedModelName = modelNames[i];
			}

			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	ImGui::NewLine();
	ImGui::Text("Control Debug Line Draw");
	auto mesh = dynamic_cast<ModelMesh*>(mainObject->mesh);
	ImGui::Checkbox("Draw Vertex Normal", &mesh->DrawVertexNormal);
	ImGui::Checkbox("Draw Face Normal", &mesh->DrawFaceNormal);

	ImGui::NewLine();
	ImGui::Text("Material");
	ImGui::ColorEdit3("Ambient", &mainObject->mesh->material->AmbientColor[0]);
	ImGui::ColorEdit3("Diffuse", &mainObject->mesh->material->DiffuseColor[0]);
	ImGui::ColorEdit3("Specular", &mainObject->mesh->material->SpecularColor[0]);

	ImGui::NewLine();
	ImGui::Text("UV Test");
	if (ImGui::Checkbox("Visualize UV", &activeTestUV))
	{
		if (activeTestUV)
		{
			mainObject->mesh->material->DiffuseTexture = TEXTURES->Get("TestUV");
			mainObject->mesh->material->SpecularTexture = TEXTURES->Get("TestUV");
		}
		else
		{
			mainObject->mesh->material->DiffuseTexture = TEXTURES->Get("Diff");
			mainObject->mesh->material->SpecularTexture = TEXTURES->Get("Spec");
		}
	}
	ImGui::End();
}

void Assignment1_Stage::UpdateLightGUI()
{
	auto selectedLight = lights[selectedLightIndex];
	ImGui::Begin("Lighting Panel");
	if (ImGui::CollapsingHeader("Light Control"))
	{
		ImGui::Text("Light Count");
		if (ImGui::SliderInt("Count", &activeLightCount, 1, 16))
			UpdateLights();

		ImGui::SeparatorEx(1);
		ImGui::Text("Orbit Rotate Active");
		ImGui::Checkbox("Active Rotation", &activeRotation);

		ImGui::SeparatorEx(1);
		ImGui::Text("Lighting Scenarios");
		if (ImGui::Button("Scenario 1"))
			SetScenario1();
		ImGui::SameLine();

		if (ImGui::Button("Scenario 2"))
			SetScenario2();
		ImGui::SameLine();

		if (ImGui::Button("Scenario 3"))
			SetScenario3();
		ImGui::SameLine();

		ImGui::NewLine();
		ImGui::SeparatorEx(1);
		ImGui::Text("Light List");
		if (ImGui::BeginCombo("Active Lights", selectedLight->Name.c_str()))
		{
			for (int i = 0; i < activeLightCount; ++i)
			{
				bool isSelected = selectedLight->Name == lights[i]->Name;
				if (ImGui::Selectable(lights[i]->Name.c_str(), isSelected))
				{
					selectedLight = lights[i];
					selectedLightIndex = i;
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		auto light = selectedLight->GetComponent<Light>();
		if (ImGui::TreeNode("Selected Light Status"))
		{
			auto prevType = lightTypeName[(int)light->Type];
			if (ImGui::BeginCombo("Type", prevType.c_str()))
			{
				for (int i = 0; i < (int)LightType::Count; ++i)
				{
					bool isSelected = prevType == lightTypeName[i];
					if (ImGui::Selectable(lightTypeName[i].c_str(), isSelected))
						light->Type = static_cast<LightType>(i);

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			ImGui::ColorEdit3("Ambient", &light->Ambient[0]);
			ImGui::ColorEdit3("Diffuse", &light->Diffuse[0]);
			ImGui::ColorEdit3("Specular", &light->Specular[0]);

			if (light->Type == LightType::Spotlight)
			{
				if (ImGui::SliderFloat("Inner Angle", &light->InnerAngle, 0, 90))
				{
					if (light->InnerAngle > light->OuterAngle)
						light->InnerAngle = light->OuterAngle;
				}

				if (ImGui::SliderFloat("Outer Angle", &light->OuterAngle, 0, 90))
				{
					if (light->OuterAngle < light->InnerAngle)
						light->OuterAngle = light->InnerAngle;
				}

				ImGui::SliderFloat("Fallout", &light->Fallout, 1, 30);
			}

			ImGui::TreePop();
		}
	}

	if (ImGui::CollapsingHeader("Global Constant Light Control"))
	{
		ImGui::DragFloat3("Attenuation Constant", &GRAPHIC->LightData.attenuationConstant[0], 0.01f, 0.0f, 2.0f);
		ImGui::ColorEdit3("Global Ambient", &GRAPHIC->LightData.globalAmbient[0]);
		ImGui::ColorEdit3("Fog Color", &GRAPHIC->LightData.fog[0]);
		ImGui::DragFloat("Fog Min Value", &GRAPHIC->LightData.minFog, 0.1f, 0.1f, 50.0f);
		ImGui::DragFloat("Fog Max Value", &GRAPHIC->LightData.maxFog, 0.1f, 0.1f, 100.0f);

		if (GRAPHIC->LightData.minFog > GRAPHIC->LightData.maxFog)
			GRAPHIC->LightData.minFog = GRAPHIC->LightData.maxFog;

		if (GRAPHIC->LightData.maxFog < GRAPHIC->LightData.minFog)
			GRAPHIC->LightData.maxFog = GRAPHIC->LightData.minFog;
	}

	ImGui::End();
}

void Assignment1_Stage::UpdateDeferredRenderGUI()
{
	ImGui::Begin("Deferred Render Panel");
	ImGui::Checkbox("Active Draw FSQ", &GRAPHIC->ActiveDrawFSQ);
	ImGui::Checkbox("Active Copy Depth Buffer", &GRAPHIC->ActiveCopyDepthBuffer);
	ImGui::End();
}

void Assignment1_Stage::UpdateDebugViewGUI()
{
	ImGui::Begin("Debug View Panel");
	GRAPHIC->DrawDeferredView();
	ImGui::End();
}

void Assignment1_Stage::SetScenario1()
{
	activeLightCount = 5;
	UpdateLights();

	glm::vec3 color = { 1,1,1 };
	for (int i = 0; i < activeLightCount; ++i)
	{
		auto light = lights[i]->GetComponent<Light>();
		light->Type = LightType::Point;
		light->Ambient = color;
		light->Diffuse = color;
		light->Specular = color;

		lights[i]->mesh->material->AmbientColor = color;
		lights[i]->mesh->material->DiffuseColor = color;
		lights[i]->mesh->material->SpecularColor = color;
	}
}

void Assignment1_Stage::SetScenario2()
{
	activeLightCount = 7;
	UpdateLights();

	std::vector<glm::vec3> colors =
	{
		{0.230f, 0.917f, 0.081f}, {0.086f, 0.350f, 0.971f,},
		{0.956f, 0.056f, 0.056f}, {0.795f, 0.113f, 0.887f},
		{0.113f, 0.887f, 0.705f}, {0.887f, 0.842f, 0.113f}, {0.887f, 0.409f, 0.113f}
	};

	for (int i = 0; i < activeLightCount; ++i)
	{
		auto light = lights[i]->GetComponent<Light>();
		light->Type = LightType::Spotlight;
		light->Ambient = colors[i];
		light->Diffuse = colors[i];
		light->Specular = colors[i];

		lights[i]->mesh->material->AmbientColor = colors[i];
		lights[i]->mesh->material->DiffuseColor = colors[i];
		lights[i]->mesh->material->SpecularColor = colors[i];
	}
}

void Assignment1_Stage::SetScenario3()
{
	activeLightCount = 11;
	UpdateLights();

	for (int i = 0; i < activeLightCount; ++i)
	{
		auto light = lights[i]->GetComponent<Light>();
		auto type = rand() % (int)LightType::Count;
		light->Type = static_cast<LightType>(type);

		float r = (float)rand() / (float)RAND_MAX;
		float g = (float)rand() / (float)RAND_MAX;
		float b = (float)rand() / (float)RAND_MAX;
		glm::vec3 color = { r,g,b };
		light->Ambient = color;
		light->Diffuse = color;
		light->Specular = color;

		lights[i]->mesh->material->AmbientColor = color;
		lights[i]->mesh->material->DiffuseColor = color;
		lights[i]->mesh->material->SpecularColor = color;
	}
}
