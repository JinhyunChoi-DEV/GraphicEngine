/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Graphic.cpp
Purpose: Making of implementing the Graphic system
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Graphic.h"
#include "Shader.h"
#include "Application.h"
#include "ObjectManager.h"
#include "Camera.h"
#include "FSQMesh.h"
#include "imgui.h"
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
	InitializeDeferredRender();

	LightData.attenuationConstant = { 0.01f, 1.44f, 0.02f };
	LightData.globalAmbient = { 0,0,0 };
	LightData.fog = { 1,1,1 };
	LightData.minFog = 0.1f;
	LightData.maxFog = 100.0f;
	ActiveDrawFSQ = true;
	ActiveCopyDepthBuffer = true;

	glEnable(GL_DEPTH_TEST);
}

void Graphic::Update()
{
	if (mainCamera == nullptr)
	{
		Object* obj = new Object("Main Camera");
		Camera* camera = new Camera(obj);
		obj->AddComponent(camera);
		OBJMANAGER->Add(obj);
		obj->transform->Position = { 0, 1, 10 };
	}

	if (ScreenSize.x <= 0 || ScreenSize.y <= 0)
		return;

	glClearColor(0, 0, 0, 1.0f);
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

		RenderDeferred(deferredRenderTargets);
		RenderForward(deferredRenderTargets, forwardRenderTargets);
	}
}

void Graphic::Close()
{
	cameras.clear();
}

void Graphic::AddLight(Object* root, Light* light)
{
	lights.push_back(std::make_pair(root, light));
}

void Graphic::DeleteLight(Light* light)
{
	for (auto it = lights.begin(); it != lights.end();)
	{
		if (it->second == light)
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

void Graphic::DrawDeferredView()
{
	ImVec2 size = { 150, 150 };
	unsigned int split = NUM_ATTACHMENT / 2;
	unsigned int count = 0;

	for (unsigned int i = 0; i < NUM_ATTACHMENT; ++i)
	{
		if (i != 0 && count == split)
			ImGui::NewLine();

		ImGui::Image((ImTextureID)gBufferTextures[i], size, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine();
		count++;
	}
}

Camera* Graphic::MainCamera()
{
	return mainCamera;
}

void Graphic::RenderDeferred(std::vector<Object*> objects)
{
	// G-Buffer Pass
	glBindFramebuffer(GL_FRAMEBUFFER, gBufferFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto const& obj : objects)
	{
		auto mesh = obj->mesh;
		mesh->Draw(obj->transform);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// for Assignment, put two object vector
void Graphic::RenderForward(std::vector<Object*> lines, std::vector<Object*> objects)
{
	// Lighting Pass
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (ActiveDrawFSQ)
	{
		fsqMesh->shader->Use();
		for (unsigned int i = 0; i < NUM_ATTACHMENT; ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, gBufferTextures[i]);
		}
		fsqMesh->Draw(nullptr);
	}

	if (ActiveCopyDepthBuffer)
	{
		// Copy Depth Buffer
		glBindFramebuffer(GL_READ_FRAMEBUFFER, gBufferFBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		glBlitFramebuffer(0, 0, (GLint)ScreenSize.x, (GLint)ScreenSize.y, 0, 0, (GLint)ScreenSize.x, (GLint)ScreenSize.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	for (auto const& obj : lines)
	{
		auto mesh = obj->mesh;
		mesh->DrawDebug(obj->transform);
	}

	for (auto const& obj : objects)
	{
		auto mesh = obj->mesh;
		mesh->Draw(obj->transform);
	}
}

void Graphic::InitializeDeferredRender()
{
	gBufferTextures = new unsigned int[NUM_ATTACHMENT];

	glGenFramebuffers(1, &gBufferFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBufferFBO);

	glGenTextures(NUM_ATTACHMENT, gBufferTextures);

	GLenum* attachments = new GLenum[NUM_ATTACHMENT];
	for (unsigned int i = 0; i < NUM_ATTACHMENT; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, gBufferTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, (GLsizei)ScreenSize.x, (GLsizei)ScreenSize.y, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, gBufferTextures[i], 0);
		attachments[i] = GL_COLOR_ATTACHMENT0 + i;
	}

	glGenRenderbuffers(1, &renderBufferDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, (GLsizei)ScreenSize.x, (GLsizei)ScreenSize.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferDepth);

	glDrawBuffers(NUM_ATTACHMENT, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Error::GBuffer" << std::endl;
		APPLICATION->IsClose = true;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	auto shader = SHADERS->Get("GBuffer");
	if (shader == nullptr)
		SHADERS->Compile("GBuffer", "GBuffer.vert", "GBuffer.frag");

	shader = SHADERS->Get("DeferredFSQ");
	if (shader == nullptr)
		SHADERS->Compile("DeferredFSQ", "LightStage.vert", "LightStage.frag");

	shader = SHADERS->Get("DeferredFSQ");
	fsqMesh = new FSQMesh();
	fsqMesh->shader = shader;
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
	for (auto const& lightP : lights)
	{
		if (lightP.first->IsActive)
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
		auto light = lights[i].second;

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
