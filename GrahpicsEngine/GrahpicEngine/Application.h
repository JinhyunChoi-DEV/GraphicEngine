#pragma once

#include <glfw/glfw3.h>

class Application
{
public:
	Application();

	void Initialize();
	void Update();
	void Close();

	const GLFWwindow* GetWindow() { return mainWindow; }

	bool IsClose;
private:
	GLFWwindow* mainWindow = nullptr;
};

extern Application* APPLICATION;