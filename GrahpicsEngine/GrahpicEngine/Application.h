/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Application.h
Purpose: Making of header file for making application.
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#pragma once

#include <glfw/glfw3.h>

class Application
{
public:
	Application();

	void Initialize();
	void Update();
	void Close();

	GLFWwindow* GetWindow() { return mainWindow; }

	bool IsClose = false;
private:
	GLFWwindow* mainWindow = nullptr;
};

extern Application* APPLICATION;