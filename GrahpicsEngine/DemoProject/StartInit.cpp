/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: StartInit.cpp
Purpose: Making of starting point of this project
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#include "Application.h"
#include "Graphic.h"
#include "Input.h"
#include "ModelLoader.h"
#include "ObjectManager.h"
#include "Assignment1_Stage.h"
#include "ShaderManager.h"
#include "TextureLoader.h"

#include <glm/glm.hpp>

int main(void)
{
	// Constructor
	Application* app = new Application();
	Graphic* graphic = new Graphic();
	Input* input = new Input();
	ModelLoader* modelLoader = new ModelLoader();
	ObjectManage* objManager = new ObjectManage();
	ShaderManager* shaderM = new ShaderManager();
	Assignment1_Stage* scene = new Assignment1_Stage();
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
