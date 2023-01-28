#include <iostream>
#include <glad/glad.h>

#include "Application.h"

#include "Graphic.h"
#include "Input.h"

void error_callback(int error_code, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double x, double y);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mode);

Application* APPLICATION = nullptr;

Application::Application()
{
	if (APPLICATION == nullptr)
		APPLICATION = this;

	glfwSetErrorCallback(error_callback);

	const int success = glfwInit();

	if (success == GLFW_FALSE)
	{
		std::cout << "Error-Application: Fail to Initialize GLFW" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mainWindow = glfwCreateWindow(1200, 800, "JinhyunChoi-GraphicEngine", NULL, NULL);
	if (mainWindow == nullptr)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_AUTO_ICONIFY, GL_FALSE);
	glfwSetKeyCallback(mainWindow, key_callback);
	glfwSetCursorPosCallback(mainWindow, mouse_callback);
	glfwMakeContextCurrent(mainWindow);
	glfwSetFramebufferSizeCallback(mainWindow, framebuffer_size_callback);
	glfwSetMouseButtonCallback(mainWindow, mouse_button_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Error-Application: Fail to Initialize GLFW" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void Application::Initialize()
{
	GRAPHIC->ScreenSize = glm::vec2(1200, 800);
}

void Application::Update()
{
	glfwSwapBuffers(mainWindow);
	glfwPollEvents();

	IsClose = glfwWindowShouldClose(mainWindow) || INPUT->KeyTriggered(GLFW_KEY_ESCAPE);
}

void Application::Close()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

void error_callback(int error_code, const char* description)
{
	fprintf(stderr, "Error: %s \n", description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	INPUT->SetKeyboard(key, action);
}

void mouse_callback(GLFWwindow* window, double x, double y)
{
	INPUT->SetMouse(x, y);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	GRAPHIC->ScreenSize = glm::vec2(width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
	INPUT->SetMouseButton(button, action);
}