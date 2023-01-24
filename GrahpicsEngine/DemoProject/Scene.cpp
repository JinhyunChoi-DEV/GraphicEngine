#include "Scene.h"

#include "Mesh.h"
#include "ObjectManager.h"
#include "ModelLoader.h"
#include "ShaderManager.h"

void Scene::Initialize()
{
	auto mesh = *MODELLOAD->Get("Test");
	mainObject = new Object();
	mainObject->AddComponent(new Mesh(mesh));

	OBJMANAGER->Add(mainObject);
}

void Scene::Update()
{
}

void Scene::Close()
{
}
