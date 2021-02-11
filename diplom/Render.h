#pragma once
#include "stdafx.h"

class DiplomApp;
class WindowManager;

class Render
{
private:
	DiplomApp* app; //��������� �� ���� ����������
	WindowManager* windowManager; //��������� �� �������� ����

	/*������� � ������ ����������� ��� �������� ������� ������*/
private:
	void createInstance(); //������ ������ �������

	VkInstance instance; //������� �������
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	bool checkValidationLayerSupport();
	//������� ���������� �������� � ������ ���������� ������
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
												const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
	//������ ������������ ���� �� ����� ������
	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

	/*������� � ������ ����������� ��� �������� ������� �����������*/
	//����������� �� ������� �� ����� �������� �����������
	VkSurfaceKHR surface;
	void createSurface();
	/*���������� ���������� ������*/
	//��������� ����������� ������
	VkDebugUtilsMessengerEXT debugMessenger;
	void setupDebugMessenger();
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

public:
	Render();
	~Render();
	void RenderInit(DiplomApp* new_app, WindowManager* new_windowManager);
};
