#include "stdafx.h"
#include "WindowManager.h"

WindowManager::WindowManager()
{

}

WindowManager::~WindowManager()
{
	if (windowCreated)
	{
		glfwDestroyWindow(window); //Уничтожаем окно
		glfwTerminate(); //Уничтожаем glfw
	}

}

void WindowManager::windowManagerInit()
{
	glfwInit();
	//Просим glfw не создавать контекст opengl
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//Создаём окно
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	//Создаём пользовательский указатель
	glfwSetWindowUserPointer(window, this);
	//Указываем какую функцию вызывать в случаи изменения окна
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	//Окно инициализированно
	windowCreated = true;
}

void WindowManager::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	//Получаем из GLFW указатель на наш класс и меняем флаг ресайза
	auto app = reinterpret_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
}

bool WindowManager::shoudClose()
{
	if (windowCreated)
		return glfwWindowShouldClose(window);
	else
		return true;
}

void WindowManager::pollEvents()
{
	if (windowCreated)
		glfwPollEvents();
	return;
}
