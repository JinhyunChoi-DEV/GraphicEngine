#include "Scene.h"
#include "Input.h"
#include "ModelMesh.h"
#include "ObjectManager.h"
#include "ModelLoader.h"

void Scene::Initialize()
{
	auto mesh = *MODELLOAD->Get("Test");
	mainObject = new Object();
	auto newMesh = new ModelMesh(mesh);
	newMesh->DrawFaceNormal = true;
	newMesh->DrawVertexNormal = true;
	newMesh->UseDeferredRendering = true;
	mainObject->mesh = newMesh;

	OBJMANAGER->Add(mainObject);
}

void Scene::Update()
{
}

void Scene::Close()
{
}
