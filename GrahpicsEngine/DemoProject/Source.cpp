#include "Application.h"
#include "Input.h"
#include "Mesh.h"
#include "ModelLoader.h"
#include "Object.h"
#include "ObjectManager.h"

class ObjectManage;

int main(void)
{
	// Constructor
	Application* app = new Application();
	Input* input = new Input();
	ModelLoader* modelLoader = new ModelLoader();
	ObjectManage* objManager = new ObjectManage();

	modelLoader->Load("Test", "cube2.obj");

	// Init
	app->Initialize();
	objManager->Initialize();

	// Update
	bool isQuit = false;
	while (!app->IsClose() && !isQuit)
	{
		input->Update();

		app->Update();
		objManager->Update();

		if (input->KeyTriggered(GLFW_KEY_ESCAPE))
			isQuit = true;
	}

	// Close
	app->Close();
}
