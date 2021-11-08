#include "stdafx.h"
#include "WindowManager.h"
#include "Camera.h"
#include "DiplomApp.h"
#include <functional>

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

void WindowManager::windowManagerInit(DiplomApp* new_app, Camera* newCamera)
{
	if (!new_app)
	{
		throw std::runtime_error("App must be created before create WindowManager");
	}
	if (!newCamera)
	{
		throw std::runtime_error("Camera must be created before create WindowManager");
	}
	app = new_app;
	camera = newCamera;
	glfwInit();
	//������ glfw �� ��������� �������� opengl
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//������ ����
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	//������ ���������������� ���������
	glfwSetWindowUserPointer(window, this);
	//��������� ����� ������� �������� � ������ ��������� ����
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	//��������� ����� ������� ������� � ������� ������� ������ ���������� � ����
	glfwSetKeyCallback(window, static_key_callback);
	//��������� ����� ������� ������� � ������� ������� ������ ����
	glfwSetMouseButtonCallback(window, static_button_callback);
	glfwSetCursorPosCallback(window, static_cursor_position_callback);
	//������� � ����, ��������� �� ��� �����
	glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));
	//���� �����������������
	windowCreated = true;
}

void WindowManager::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	//�������� �� GLFW ��������� �� ��� ����� � ������ ���� �������
	auto app = reinterpret_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
}

void WindowManager::static_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//�������� ��� ��������� ����� �� ���� � �������� ���
	WindowManager* handler = reinterpret_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	if (handler)
	{
		handler->key_callback(key, scancode, action, mods);
	}		
}

void WindowManager::key_callback(int key, int scancode, int action, int mods)
{
	//std::cout << "some one press a key: " << key << " scancode: " << scancode << " action: " << action << " mods: " << mods << std::endl;
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	switch (key)
	{
	case(GLFW_KEY_W):
	{
		if (action)
			x = 1.0f;
		else
			x = 0.0f;
		break;
	}
	case(GLFW_KEY_S):
	{
		if (action)
			x = -1.0f;
		else
			x = 0.0f;
		break;
	}
	case(GLFW_KEY_D):
	{
		if (action)
			y = 1.0f;
		else
			y = 0.0f;
		break;
	}
	case(GLFW_KEY_A):
	{
		if (action)
			y = -1.0f;
		else
			y = 0.0f;
		break;
	}
	case(GLFW_KEY_E):
	{
		if (action)
			z = 1.0f;
		else
			z = 0.0f;
		break;
	}
	case(GLFW_KEY_Q):
	{
		if (action)
			z = -1.0f;
		else
			z = 0.0f;
		break;
	}
	default: {break; }
	}

	camera->inputAxis = { x, y, z };
}

void WindowManager::static_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	WindowManager* handler = reinterpret_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	if (handler)
	{
		handler->button_callback(button, action, mods);
	}
}
void WindowManager::button_callback(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
		{
			//����������� ������ ����� � �������� ������ ���� � �����
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
			buttonPressed = true;
		}
		else
		{
			glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			buttonPressed = false;
			lastPos = { 0.0, 0.0 };
		}
	}
}
void WindowManager::static_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	WindowManager* handler = reinterpret_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	if (handler)
	{
		handler->cursor_position_callback(xpos, ypos);
	}
}

void WindowManager::cursor_position_callback(double xpos, double ypos)
{
	if (!buttonPressed)
		return;

	if (lastPos.x == 0.0 && lastPos.y == 0.0)
	{
		lastPos = { xpos, ypos };
		return;
	}

	glm::dvec2 deltaPos = lastPos - glm::dvec2{ xpos, ypos };
	double sensitivity = 0.1;
	deltaPos *= sensitivity;
	camera->rotationAxis += deltaPos ;
	lastPos = { xpos, ypos };
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
