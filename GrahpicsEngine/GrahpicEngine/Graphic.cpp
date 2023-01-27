#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Graphic.h"
#include "ObjectManager.h"
#include "Camera.h"
#include "Object.h"
#include "ModelMesh.h"
#include "ShaderManager.h"
#include "Transform.h"
#include "Light.h"

Graphic* GRAPHIC = nullptr;
Graphic::Graphic()
{
	if (GRAPHIC == nullptr)
		GRAPHIC = this;
}

void Graphic::Initialize()
{
	InitializeUniformBuffer();

	//TODO: just make in scene code and update function check if it is null then make
	if (mainCamera == nullptr)
	{
		Object* obj = new Object("Main Camera");
		Camera* camera = new Camera(obj);
		obj->AddComponent(camera);
		OBJMANAGER->Add(obj);
		obj->transform->Position = { 0, 1, 15 };
	}

	LightData.attenuationConstant = { 1.0f, 0.22f, 0.2f };
	LightData.globalAmbient = { 0,0,0 };
	LightData.fog = { 0,0,0 };
	LightData.minFog = 0.1f;
	LightData.maxFog = 25.0f;

	glEnable(GL_DEPTH_TEST);
}

void Graphic::Update()
{
	glClearColor(LightData.fog.r, LightData.fog.g, LightData.fog.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, static_cast<int>(ScreenSize.x), static_cast<int>(ScreenSize.y));

	auto objects = OBJMANAGER->GetAll();
	std::vector<Object*> deferredRenderTargets;
	std::vector<Object*> forwardRenderTargets;

	UpdateLightingUniformBuffer();

	for (auto const& cam : cameras)
	{
		UpdateTransformUniformBuffer(cam);

		deferredRenderTargets.clear();
		forwardRenderTargets.clear();

		for (auto obj : objects)
		{
			if (!obj->IsActive)
				continue;

			if (obj->mesh == nullptr)
				continue;

			if (obj->mesh->UseDeferredRendering)
				deferredRenderTargets.push_back(obj);
			else
				forwardRenderTargets.push_back(obj);
		}

		//TODO: fix logic for deferred Rendering 
		for (auto const& obj : deferredRenderTargets)
		{
			auto mesh = obj->mesh;
			mesh->Draw(obj->transform);
		}


		//TODO: fix logic for forward Rendering
		// for the Assignment
		for (auto const& obj : deferredRenderTargets)
		{
			auto mesh = obj->mesh;
			mesh->DrawDebug(obj->transform);
		}

		for (auto const& obj : forwardRenderTargets)
		{
			auto mesh = obj->mesh;
			mesh->Draw(obj->transform);
		}
	}
}

void Graphic::Close()
{
	cameras.clear();
}

void Graphic::AddLight(Light* light)
{
	lights.push_back(light);
}

void Graphic::DeleteLight(Light* light)
{
	for (auto it = lights.begin(); it != lights.end();)
	{
		if (*it == light)
		{
			lights.erase(it);
			break;
		}

		++it;
	}
}

void Graphic::AddCamera(Camera* camera)
{
	auto size = static_cast<int>(cameras.size());
	if (size <= 0)
		mainCamera = camera;

	if (camera->IsMain)
	{
		for (auto const& cam : cameras)
		{
			if (cam->IsMain)
				cam->IsMain = false;
		}
	}

	cameras.push_back(camera);
}

void Graphic::DeleteCamera(Camera* target)
{
	for (auto it = cameras.begin(); it != cameras.end();)
	{
		if (*it == target)
		{
			cameras.erase(it);
			break;
		}

		++it;
	}
}

const Camera* Graphic::MainCamera()
{
	return mainCamera;
}

void Graphic::InitializeUniformBuffer()
{
	// transform Uniform Block
	glGenBuffers(1, &uboTransform);
	glBindBuffer(GL_UNIFORM_BUFFER, uboTransform);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboTransform, 0, sizeof(glm::mat4) * 2);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// lighting Uniform Block
	int size = 112 * 16 + 64;
	glGenBuffers(1, &uboLighting);
	glBindBuffer(GL_UNIFORM_BUFFER, uboLighting);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, uboLighting, 0, size);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Graphic::UpdateTransformUniformBuffer(Camera* cam) const
{
	auto view = cam->View();
	auto projection = cam->Projection();

	glBindBuffer(GL_UNIFORM_BUFFER, uboTransform);

	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Graphic::UpdateLightingUniformBuffer()
{
	int activeCount = 0;
	for (auto const& light : lights)
	{
		if (light->IsActive)
			activeCount++;
	}

	glBindBuffer(GL_UNIFORM_BUFFER, uboLighting);

	GLint offset = 0;
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(unsigned int), &activeCount);
	offset += 4;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &LightData.minFog);
	offset += 4;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &LightData.maxFog);
	offset += 8;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(LightData.attenuationConstant));
	offset += 16;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(LightData.globalAmbient));
	offset += 16;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(LightData.fog));
	offset += 16;

	for (int i = 0; i < activeCount; ++i)
	{
		auto light = lights[i];

		glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(unsigned int), &light->Type);
		offset += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(light->transform->Position));
		offset += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(light->Direction));
		offset += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(light->Ambient));
		offset += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(light->Diffuse));
		offset += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(light->Specular));
		offset += 12;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &light->InnerAngle);
		offset += 4;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &light->OuterAngle);
		offset += 4;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &light->Fallout);
		offset += 12;
	}

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
