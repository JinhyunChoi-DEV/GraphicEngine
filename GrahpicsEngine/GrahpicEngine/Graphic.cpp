#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Graphic.h"
#include "ObjectManager.h"
#include "Camera.h"
#include "Object.h"
#include "Mesh.h"
#include "Transform.h"

Graphic* GRAPHIC = nullptr;
Graphic::Graphic()
{
	if (GRAPHIC == nullptr)
		GRAPHIC = this;
}

void Graphic::Initialize()
{
	InitializeUniformBuffer();

	if (mainCamera == nullptr)
	{
		Object* obj = new Object("Main Camera");
		Camera* camera = new Camera(obj);
		obj->AddComponent(camera);
		OBJMANAGER->Add(obj);
		obj->transform->Position = { 0, 0, 15 };
	}

	glEnable(GL_DEPTH_TEST);
}

void Graphic::Update()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, static_cast<int>(ScreenSize.x), static_cast<int>(ScreenSize.y));

	auto objects = OBJMANAGER->GetAll();
	std::vector<Object*> deferredRenderTargets;
	std::vector<Object*> forwardRenderTargets;

	for (auto const& cam : cameras)
	{
		UpdateUniformBuffer(cam);

		deferredRenderTargets.clear();
		forwardRenderTargets.clear();

		for (auto obj : objects)
		{
			auto mesh = obj->GetComponent<Mesh>();
			if (mesh == nullptr)
				continue;
			if (mesh->UseDeferredRendering)
				deferredRenderTargets.push_back(obj);
			else
				forwardRenderTargets.push_back(obj);
		}

		//TODO: fix logic for deferred Rendering 
		for (auto const& obj : deferredRenderTargets)
		{
			auto mesh = obj->GetComponent<Mesh>();
			mesh->Draw(obj->transform);
		}

		//TODO: fix logic for forward Rendering
		for (auto const& obj : forwardRenderTargets)
		{
			auto mesh = obj->GetComponent<Mesh>();
			mesh->Draw(obj->transform);
		}
	}
}

void Graphic::Close()
{
	cameras.clear();
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

void Graphic::InitializeUniformBuffer()
{
	glGenBuffers(1, &uboMatrix);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrix);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrix, 0, sizeof(glm::mat4) * 2);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Graphic::UpdateUniformBuffer(Camera* cam) const
{
	auto view = cam->View();
	auto projection = cam->Projection();

	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrix);

	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
