#pragma once
#include "stdafx.h"
#include "HelpStructures.h"

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
	/*������� � ������ ����������� ��� �������� ����������� ����������*/
	//������� ����������� ����������
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	//������ �������������� ������ ����������
	VkPhysicalDeviceFeatures deviceFeatures{};
	//������ ����������� ���������� ��� ����������
	const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	/*������� � ������ ����������� ��� �������� ����������� ����������*/
	void createLogicalDevice();
	//������ � ����������� ������ �������
	//��������� ����������� ����������
	VkDevice device;
	//��������� ������� ��� ���������
	VkQueue graphicsQueue;
	//��������� ������� ��� ������
	VkQueue presentQueue;
	//��������� ������� ��� ������
	VkQueue transfertQueue;
	/*������� � ������ ����������� ��� �������� ������� ������*/
	void createSwapChain();
	void cleanupSwapChain();
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	//������� ������
	VkSwapchainKHR swapChain;
	//�������� ����������� �� ������� ������
	std::vector<VkImage> swapChainImages;
	//������ �����������
	VkFormat swapChainImageFormat;
	//������� �����������
	VkExtent2D swapChainExtent;
public:
	Render();
	~Render();
	void RenderInit(DiplomApp* new_app, WindowManager* new_windowManager);

};
