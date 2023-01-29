#include "Application.h"
#include "Graphic.h"
#include "Input.h"
#include "Mesh.h"
#include "ModelLoader.h"
#include "ModelMesh.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "ShaderManager.h"
#include "TextureLoader.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

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
	TextureLoader* textureLoader = new TextureLoader();

	shaderM->Compile("Default", "Solid.vert", "Solid.frag");
	shaderM->Compile("Line", "Line.vert", "Line.frag");
	shaderM->Compile("Phong", "Phong Shading.vert", "Phong Shading.frag");


	// Init
	app->Initialize();
	graphic->Initialize();
	objManager->Initialize();
	scene->Initialize();

	// Update
	while (!app->IsClose)
	{
		input->Update();

		app->Update();
		objManager->Update();
		graphic->Update();
		scene->Update();
	}

	// Close
	scene->Close();
	graphic->Close();
	objManager->Close();
	app->Close();

	// clean up memory
	delete app;
	delete graphic;
	delete input;
	delete modelLoader;
	delete objManager;
	delete shaderM;
	delete scene;
}
