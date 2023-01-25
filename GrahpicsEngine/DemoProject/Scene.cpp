#include "Scene.h"

#include "Camera.h"
#include "Graphic.h"
#include "Input.h"
#include "Mesh.h"
#include "ObjectManager.h"
#include "ModelLoader.h"
#include "ShaderManager.h"

void Scene::Initialize()
{
	auto mesh = *MODELLOAD->Get("Test");
	mainObject = new Object();
	auto newMesh = new Mesh(mesh);
	newMesh->UseDeferredRendering = true;
	mainObject->AddComponent(newMesh);

	OBJMANAGER->Add(mainObject);
}

void Scene::Update()
{
}

void Scene::Close()
{
}
