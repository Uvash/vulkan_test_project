#include "stdafx.h"
#include "WindowManager.h"
#include "DiplomApp.h"

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

void WindowManager::windowManagerInit(DiplomApp* new_app)
{
	if (new_app == nullptr)
	{
		throw std::runtime_error("App must be created before create WindowManager");
	}
	app = new_app;
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

std::vector<const char*> WindowManager::getRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	//Опрашиваем glfw о необходимых ему расширениях
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	//Выделяем память под список расширений
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	return extensions;
}

void WindowManager::createSurface(VkInstance& instance, VkSurfaceKHR& surface)
{
	//Создаёт поверхность выбор поверхности и платформы отдаём на откуп glfw
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}
}

void WindowManager::getFramebufferSize(int &width, int &height)
{
	glfwGetFramebufferSize(window, &width, &height);
}

void WindowManager::setFramebufferResized(bool newFlag)
{
	framebufferResized = newFlag;
}
