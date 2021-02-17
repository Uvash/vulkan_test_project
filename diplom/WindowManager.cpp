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
		glfwDestroyWindow(window); //���������� ����
		glfwTerminate(); //���������� glfw
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
	//������ glfw �� ��������� �������� opengl
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//������ ����
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	//������ ���������������� ���������
	glfwSetWindowUserPointer(window, this);
	//��������� ����� ������� �������� � ������ ��������� ����
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	//���� �����������������
	windowCreated = true;
}

void WindowManager::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	//�������� �� GLFW ��������� �� ��� ����� � ������ ���� �������
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
	//���������� glfw � ����������� ��� �����������
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	//�������� ������ ��� ������ ����������
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	return extensions;
}

void WindowManager::createSurface(VkInstance& instance, VkSurfaceKHR& surface)
{
	//������ ����������� ����� ����������� � ��������� ����� �� ����� glfw
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
