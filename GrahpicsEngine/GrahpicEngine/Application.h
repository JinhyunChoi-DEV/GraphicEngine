#pragma once

#include <glfw/glfw3.h>

class Application
{
public:
	Application();

	void Initialize();
	void Update();
	void Close();

	bool IsClose() { return glfwWindowShouldClose(mainWindow); }
	const GLFWwindow* GetWindow() { return mainWindow; }

private:
	GLFWwindow* mainWindow = nullptr;
};

extern Application* APPLICATION;