#include "Application.h"
#include "Graphic.h"
#include "Input.h"
#include "Mesh.h"
#include "ModelLoader.h"
#include "ModelMesh.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "ShaderManager.h"
#include <typeinfo>

int main(void)
{
	// Constructor
	Application* app = new Application();
	Graphic* graphic = new Graphic();
	Input* input = new Input();
	ModelLoader* modelLoader = new ModelLoader();
	ObjectManage* objManager = new ObjectManage();
	ShaderManager* shaderM = new ShaderManager();
	Scene* scene = new Scene();

	shaderM->Compile("Default", "Solid.vert", "Solid.frag");
	shaderM->Compile("Line", "Line.vert", "Line.frag");
	shaderM->Compile("Phong", "Phong Shading.vert", "Phong Shading.frag");

	// Init
	app->Initialize();
	graphic->Initialize();
	objManager->Initialize();
	scene->Initialize();

	// Update
	bool isQuit = false;
	while (!app->IsClose() && !isQuit)
	{
		input->Update();

		app->Update();
		objManager->Update();
		scene->Update();
		graphic->Update();

		if (input->KeyTriggered(GLFW_KEY_ESCAPE))
			isQuit = true;
	}

	// Close
	app->Close();
	graphic->Close();
	objManager->Close();
	scene->Close();

	// clean up memory
	delete app;
	delete graphic;
	delete input;
	delete modelLoader;
	delete objManager;
	delete shaderM;
	delete scene;
}
