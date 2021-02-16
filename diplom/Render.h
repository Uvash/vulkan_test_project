#pragma once
#include "stdafx.h"
#include "HelpStructures.h"

class DiplomApp;
class WindowManager;

class Render
{
private:
	DiplomApp* app; //Указатель на наше приложение
	WindowManager* windowManager; //Указатель на менеджер окон

	/*ФУНКЦИИ И МЕТОДЫ НЕОБХОДИМЫЕ ДЛЯ СОЗДАНИЯ ОБЪЕКТА ВУЛКАН*/
private:
	void createInstance(); //Создаёт объект Вулкана

	VkInstance instance; //Хендлер Вулкана
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	bool checkValidationLayerSupport();
	//Каллбек вызываемый вулканом в случаи нахождения ошибок
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
												const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
	//Список подключаемых слоёв во время дебага
	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

	/*ФУНКЦИИ И МЕТОДЫ НЕОБХОДИМЫЕ ДЛЯ СОЗДАНИЯ ОБЪЕКТА ПОВЕРХНОСТЬ*/
	//Поверхность на которую мы будем выводить изображение
	VkSurfaceKHR surface;
	void createSurface();
	/*Подключаем обработчик ошибок*/
	//экземпляр обработчика ошибок
	VkDebugUtilsMessengerEXT debugMessenger;
	void setupDebugMessenger();
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
	/*ФУНКЦИИ И МЕТОДЫ НЕОБХОДИМЫЕ ДЛЯ СОЗДАНИЯ ФИЗИЧЕСКОГО УСТРОЙСТВА*/
	//Хендлер физического устройства
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	//Список дополнительных плюшек устройства
	VkPhysicalDeviceFeatures deviceFeatures{};
	//Список необходимых расширений для устройства
	const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	/*ФУНКЦИИ И МЕТОДЫ НЕОБХОДИМЫЕ ДЛЯ СОЗДАНИЯ ЛОГИЧЕСКОГО УСТРОЙСТВА*/
	void createLogicalDevice();
	//Вместе с устройством создаём очереди
	//Экземпляр логического устройства
	VkDevice device;
	//Экземпляр очереди для отрисовки
	VkQueue graphicsQueue;
	//Экземпляр очереди для показа
	VkQueue presentQueue;
	//Экземпляр очереди для обмена
	VkQueue transfertQueue;
	/*ФУНКЦИИ И МЕТОДЫ НЕОБХОДИМЫЕ ДЛЯ СОЗДАНИЯ ЦЕПОЧКИ ОБМЕНА*/
	void createSwapChain();
	void cleanupSwapChain();
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	//Цепочка обмена
	VkSwapchainKHR swapChain;
	//Хендлеры изображений из цепочки обмена
	std::vector<VkImage> swapChainImages;
	//Формат изображений
	VkFormat swapChainImageFormat;
	//Размеры изображений
	VkExtent2D swapChainExtent;
public:
	Render();
	~Render();
	void RenderInit(DiplomApp* new_app, WindowManager* new_windowManager);

};
