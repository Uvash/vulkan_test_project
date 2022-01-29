#include "stdafx.h"
#include "Render.h"
#include "renderHelp/ShaderLoader.h"
#include "renderHelp/StaticStage.h"
#include "renderHelp/GraphicsPipeline.h"
#include "renderHelp/DescriptorSetLayout.h"
#include "renderHelp/HelpStructures.h"
#include "renderHelp/PipelineInfo.h"
#include "renderHelp/PipelineInfo/Line.h"
#include "renderHelp/PipelineInfo/StripLine.h"
#include "Buffer.h"
#include "ExpandBufferDeque.h"
#include "Primitive/Primitive.h"
#include "DiplomApp.h"
#include "WindowManager.h"



Render::Render()
{

}
Render::~Render()
{
	cleanupSwapChain();
	descriptorSetLayout.reset();

	swapForVertexBuffer.reset();
	vertexBuffer.reset();
	primitive.reset();

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}

	vkDestroyCommandPool(device, commandPool, nullptr);
	vkDestroyCommandPool(device, transfertСommandPool, nullptr);

	vkDestroyDevice(device, nullptr);
	if (app->enableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}
	vkDestroySurfaceKHR(instance, surface, nullptr); //Уничтожаем поверхность вывода
	vkDestroyInstance(instance, nullptr); //Уничтожаем экземпляр вулкана

	primitive.reset();
}

void Render::RenderInit(DiplomApp* new_app, WindowManager* new_windowManager, AbstractItertionMetod* newItertionMetod, Camera* newCamera)
{
	if(!new_app)
	{
		throw std::runtime_error("App must be created before create Vulkan");
	}
	if(!new_windowManager)
	{
		throw std::runtime_error("WindowManager must be created before create Vulkan");
	}
	if (!newItertionMetod)
	{
		throw std::runtime_error("WindowManager must be created before create Vulkan");
	}
	if (!newCamera)
	{
		throw std::runtime_error("Camera must be created before create Vulkan");
	}
	app = new_app;
	windowManager = new_windowManager;
	itertionMetod = newItertionMetod;
	camera = newCamera;

	createInstance();
	createSurface();
	setupDebugMessenger();
	pickPhysicalDevice();
	createLogicalDevice();
	createSwapChain();
	createImageViews();
	createRenderPass();
	createDescriptorSetLayout();
	createPipelineLayout();
	createGraphicsPipeline();
	createFramebuffers();
	initCommandPool();
	createVertexBuffer();
	createIndexBuffer();
	createUniformBuffers();
	createDescriptorPool(); 
	createDescriptorSets();
	createCommandBuffers();
	createSyncObjects();
}

void Render::createInstance()
{
	VkApplicationInfo appInfo{}; //Структура содержит информацию о нашем приложении
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{}; //Структура содержит информацию для создания экземпляра

	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	//Получаем список запрашиваемых расширений у приложения
	auto extensions = app->getRequiredExtensions();
	//Указываем кол-во расширений и указатель на первое.
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	//Проверяем наличие запрашиваемых слоёв в дебаг моде
	if (app->enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}
	//Создаём структуру описывающую обработчик ошибок
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	//Подключаем слои
	if (app->enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
		//заполняем структуру обработчика ошибок
		populateDebugMessengerCreateInfo(debugCreateInfo);
		//связываем обработчик ошибок с экземпляром вулкана
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;

	}
	else
	{
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}


	//Информацию собрали. Пробуем создать экземпляр
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create instance!");
	}

}

void Render::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType =
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;

}

/*
 *@brief Обработчик ошибок, вызываемх вулканом
 *
 *@param  messageSeverity Бит серьёзности сообщения (диагностическое, информационное при создании, предупреждение, ошибка)
 *@param  messageType Бит типа сообщения (основной тип, предупреждение/ошибка, не оптимальное использование вулкана)
 *@param  pCallbackData Структура описывающая детали ошибки
 *@param  pUserData Дополнительные данные которые мы передаём
 *
 *@return Возвращает флаг о необходимости прерывания выполнения вызывающей функцией
 *
 */
VKAPI_ATTR VkBool32 VKAPI_CALL Render::debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData)
{
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
	return VK_FALSE;
}


bool Render::checkValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr); //Узнаём количество слоёв поддерживаемое вулканом

	std::vector<VkLayerProperties> availableLayers(layerCount); //Выделяем память под слои

	vkEnumerateInstanceLayerProperties(&layerCount,
		availableLayers.data()); //Записываем слои в вектор

	//Обходим список запрашиваемых слоёв
	for (const char* layerName : validationLayers)
	{
		bool layerFound = false;
		//Организуем поиск по доступным слоям
		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}

		}
		//Мы обошли все слои но не нашли нужного
		if (!layerFound)
		{
			return false;
		}
	}

	return true;
}

void Render::createSurface()
{
	windowManager->createSurface(instance, surface);
}

void Render::setupDebugMessenger()
{
	if (!app->enableValidationLayers)
		return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);
	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to set up debug messenger!");
	}
}
/*
 *@brief Создаём обработчик ошибок, подгружая его из дополнительных расширений
 *
 *@param  instance экземпляр вулкана
 *@param  pCreateInfo структура, содержащая информацию необходимую для конструирования обработчика
 *@param  pAllocator дополнительный аллокатор
 *@param  pDebugMessenger место куда будет записан обработчик ошибок
 *
 *@return Возвращает статус операции
 */
VkResult Render::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	// vkGetInstanceProcAddr возвращает указатель на фунцию, если она может быть загружена
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

}
/*
 *@brief Уничтожаем обработчик, вызывая функции из дополнительных расширений
 *
 *@param  instance экземпляр вулкана
 *@param  pDebugMessenger обработчик ошибок
 *@param  pAllocator дополнительный аллокатор
 */
void Render::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		func(instance, debugMessenger, pAllocator);
	}
}

void Render::pickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	//Ищем подходящие под наши задачи устройство
	for (const auto& device : devices)
	{
		if (isDeviceSuitable(device))
		{
			physicalDevice = device;
			break;
		}

	}
	//если не смогли найти устройство
	if (physicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

bool Render::isDeviceSuitable(VkPhysicalDevice device)
{
	//Нам нужно устройство с выводом графики
	QueueFamilyIndices indices = findQueueFamilies(device);
	bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported)
	{
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}
	return indices.isComplete() && extensionsSupported && swapChainAdequate;

}

QueueFamilyIndices Render::findQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	//Узнаём и загружаем очереди
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
		nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
		queueFamilies.data());
	//Ищем необходимую очередь
	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		//Проверяем может ли текущая очередь поддерживать показ на поверхности
		VkBool32 presentSupport = false;

		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (presentSupport && !(indices.presentFamily.has_value()))
		{
			indices.presentFamily = i;
		}
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT && !(indices.graphicsFamily.has_value()))
		{
			indices.graphicsFamily = i;
		}
		// Мы будем использовать для обмена очередь содержащую VK_QUEUE_TRANSFER_BIT и не содержащую VK_QUEUE_GRAPHICS_BIT
		if ((queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) && !(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT))
		{
			indices.transferFamily = i;
		}

		//Все очереди подготовленны
		if (indices.isComplete())
		{
			break;
		}
		i++;
	}
	return indices;
}

bool Render::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	//Запрашиваем расширения, поддерживаемые устройством
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
	//Обходим доступные расширения удаляя найденые
	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}
	//После цикла остаются не поддерживаемые расширения
	return requiredExtensions.empty();
}

/*
 *@brief Представляет необходимую информацию для проверки совместимости цепочек обмена и системы
 *
 *@param device физическое устройство поддерживающие вулкан
 *@return Структура с информацией о цепочке обмена
 */
SwapChainSupportDetails Render::querySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}
	return details;
}

void Render::createLogicalDevice()
{
	//Структура описивающая информацию для создания очереди
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value(), indices.transferFamily.value() };
	//Задаём приоритет очереди
	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pEnabledFeatures = &deviceFeatures;
	//Подключаем запрашиваемые расширения
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (app->enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}
	//Всё подготовили. Пытаемся создать логическое устройство
	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create logical device!");
	}
	//Подгружаем очередь поскольку она одна. То у неё индекс 0
	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
	vkGetDeviceQueue(device, indices.transferFamily.value(), 0, &transfertQueue);
}

void Render::createSwapChain()
{
	//Проверяем и подготавливаем данные для структуры описывающей Swapchain
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);
	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

	//Сохраняем ормат и размеры изображения для будущего
	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;

	//Указываем количество изображений в цепочке показа
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

	//Проверяем не привысили ли мы лимит 0 - означает что лимита нет
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	//Начинаем создавать структуру для цепочки обмена
	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1; //Количество слоёв у изображения
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; //Назначение изображения (отрисовка на экране)

	//Подключаем очереди команд
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional

	}
	//Указываем необходимость дополнительного преобразования. Точнее его отсутствии
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	//Отключаем альфа канал
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	createInfo.presentMode = presentMode;
	//Указываем, что необходимо обрезать не показываемаемые пиксели
	createInfo.clipped = VK_TRUE;
	//Указатель на передущую цепочку обмена (по идее если мы пересоздаём её с новыми параметрами)
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	//Создаём цепочку
	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create swap chain!");
	}
	//Сразу заполним дескрипторы изображений
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());
}

void Render::recreateSwapChain()
{
	int width = 0, height = 0;
	//Узнаём размеры окна и если они нулевые ждём разворота
	windowManager->getFramebufferSize(width, height);
	while (width == 0 || height == 0)
	{
		windowManager->getFramebufferSize(width, height);
		glfwWaitEvents();
	}
	camera->setProj(width / (float)height);
	//Ожидаем завершения всех операций
	vkDeviceWaitIdle(device);
	//Удаляем кадры и прочий мусор
	cleanupSwapChain();

	createSwapChain();
	createImageViews();
	createRenderPass();
	createPipelineLayout();
	createGraphicsPipeline();
	createFramebuffers();
	createUniformBuffers();
	createDescriptorPool();
	createDescriptorSets();
	createCommandBuffers();

}

void Render::cleanupSwapChain()
{

	for (size_t i = 0; i < swapChainFramebuffers.size(); i++)
	{
		vkDestroyFramebuffer(device, swapChainFramebuffers[i], nullptr);
	}
	vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

	graphicsPipelines.clear();

	destroyPipelineLayout();
	vkDestroyRenderPass(device, renderPass, nullptr);
	for (size_t i = 0; i < swapChainImageViews.size(); i++)
	{
		vkDestroyImageView(device, swapChainImageViews[i], nullptr);
	}

	for (size_t i = 0; i < swapChainImages.size(); i++)
	{
		uniformBuffers[i].reset();
	}

	vkDestroyDescriptorPool(device, descriptorPool, nullptr);
	vkDestroySwapchainKHR(device, swapChain, nullptr);
}
/*
 *@brief Выбирает необходимый цветовой формат из доступных
 *
 *@param availableFormats доступные цветовые форматы
 *@return выбранный цветовой формат
 */
VkSurfaceFormatKHR Render::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}
	return availableFormats[0];
}

/*
 *@brief Функция выбора предпочтительного режима показа
 *
 *@param availablePresentModes доступные режимы показа
 *@return режим показа
 */
VkPresentModeKHR Render::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
	}
	return VK_PRESENT_MODE_FIFO_KHR; //Этот режим доступен всегда
}

/*
 *@brief Функция выбора ширины и высоты
 *
 *@param capabilities параметры цепочки показа
 *@return текущие размеры
 */
VkExtent2D Render::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != UINT32_MAX)
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width, height;

		windowManager->getFramebufferSize(width, height);
		camera->setProj(width / (float)height); // обновляем соотношение сторон на экране
		VkExtent2D actualExtent =
		{
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};
	}
}

void Render::createImageViews()
{
	swapChainImageViews.resize(swapChainImages.size());

	for (size_t i = 0; i < swapChainImages.size(); i++)
	{
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = swapChainImages[i];
		//Тип представления
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = swapChainImageFormat;
		//Настраиваем цветовые каналы
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		//Указываем что фичи нам не нужны
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		//Создаём изображение
		if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create image views!");
		}
	}

}

void Render::createRenderPass()
{
	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create render pass!");
	}
}

void Render::createDescriptorSetLayout()
{
	descriptorSetLayout = std::make_shared<DescriptorSetLayout>(device);
}

void Render::createGraphicsPipeline()
{
	std::vector<std::shared_ptr<PipelineInfo>> pipelinePrecursor;
	pipelinePrecursor.push_back(std::make_shared<Pipeline::StripLine>(*this));
	pipelinePrecursor.push_back(std::make_shared<Pipeline::Line>(*this));

	for (auto pipeline : pipelinePrecursor)
	{
		pipeline->assemblePipelineInfo();
		graphicsPipelines.push_back(GraphicsPipeline{ device, &pipeline->getPipelineInfo() });
	}	
}
void Render::createPipelineLayout()
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout->getDescriptorSetLayout();
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional

	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional
	if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create pipeline layout!");
	}
}

void Render::destroyPipelineLayout()
{
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
}
void Render::createFramebuffers()
{
	//Узнаём необходимое кол-во кадров
	swapChainFramebuffers.resize(swapChainImageViews.size());
	//Создаём кадры и указываем их совместимость с проходами рендеринга
	for (size_t i = 0; i < swapChainImageViews.size(); i++)
	{
		VkImageView attachments[] = { swapChainImageViews[i] };

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create framebuffer!");
		}

	}
}

void  Render::initCommandPool()
{
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

	createCommandPool(queueFamilyIndices.graphicsFamily.value(), &commandPool);
	createCommandPool(queueFamilyIndices.transferFamily.value(), &transfertСommandPool);
}

/*
 *@brief создаёт командный пулл
 *
 *@param familyIndex номер очереди
 *@param pool пустой командрный пулл
 *@return
 */
void Render::createCommandPool(uint32_t familyIndex, VkCommandPool* pool)
{

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = familyIndex; //Указываем в какую очередь мы будем слать команды
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Optional

	if (vkCreateCommandPool(device, &poolInfo, nullptr, pool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create command pool!");
	}

}

void Render::createVertexBuffer()
{
//Удалить после тестов
#if 0
	std::vector<glm::vec3> rawData = { {-20.0f , 0.0f , 0.0f}, {0.0f, 1.0f, 1.0f}, {20.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f , -30.0f , 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 30.0f, 0.0f}, {0.0f, 1.0f, 0.0f},  {0.0f , 0.0f , 0.0f}, {0.0f, 1.0f, 0.0f} , {0.0f, 0.0f, 50.0f} , {0.0f, 1.0f, 0.0f} };
	primitive = std::make_shared<Primitive>(*this);
	VkDeviceSize primitiveSize = (VkDeviceSize)(rawData.size() * sizeof(rawData[0]));
	primitive->pBuffer->createBuffer(primitiveSize,VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	vkQueueWaitIdle(graphicsQueue);

	void* data;
	vkMapMemory(device, primitive->pBuffer->getVkMemoryHandle(), 0, primitiveSize, 0, &data);
	memcpy(data, rawData.data(), primitiveSize);
	vkUnmapMemory(device, primitive->pBuffer->getVkMemoryHandle());

#endif
	primitive = std::make_shared<Primitive>(*this);

	primitive->loadFromFile("shapes/AxisColored.txt");

	vertexBuffer = std::make_shared<ExpandBufferDeque>(*this);

	vertexBuffer->setHotBufferSpecificParameters(VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	vertexBuffer->setColdBufferSpecificParameters(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	vertexBuffer->createBuffer(sizeof(itertionMetod->result[0]), 1024);

	//создаём буфер для обмена
	swapForVertexBuffer = std::make_shared<Buffer>(*this);
	swapForVertexBuffer->createBuffer(sizeof(itertionMetod->result[0]), VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

void Render::createIndexBuffer()
{

}

void Render::createUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize(swapChainImages.size());

	for (size_t i = 0; i < swapChainImages.size(); i++)
	{
		uniformBuffers[i] = std::make_shared<Buffer>(*this);
		uniformBuffers[i]->createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}
}

void Render::updateUniformBuffer(uint32_t currentImage)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	//Изменяем временный буфер для кадра в зависимости от времени

	camera->getUniformBuffer();
	//Пишем переносим временный буфер в глобальный буфер кадра
	void* data;
	vkMapMemory(device, uniformBuffers[currentImage]->getVkMemoryHandle(), 0, sizeof(*camera->getUniformBuffer()), 0, &data);
	memcpy(data, camera->getUniformBuffer(), sizeof(*camera->getUniformBuffer()));
	vkUnmapMemory(device, uniformBuffers[currentImage]->getVkMemoryHandle());
}

void Render::createDescriptorPool()
{

	VkDescriptorPoolSize poolSize{};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = static_cast<uint32_t>(swapChainImages.size());

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = static_cast<uint32_t>(swapChainImages.size());


	if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void Render::createDescriptorSets()
{
	std::vector<VkDescriptorSetLayout> layouts(swapChainImages.size(), descriptorSetLayout->getDescriptorSetLayout());
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImages.size());
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(swapChainImages.size());
	if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < swapChainImages.size(); i++)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i]->getVkBufferHandle();
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;
		descriptorWrite.pImageInfo = nullptr; // Optional
		descriptorWrite.pTexelBufferView = nullptr; // Optional

		vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);

	}
}

void Render::expandVertexBuffer()
{
	static auto startTime = std::chrono::high_resolution_clock::now();
	static auto lastTime = startTime;
	static int rawOffset = 6;
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	float lastPeriod = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();

	if (itertionMetod->needUpdate())
	{
		//КОСТЫЛЬ Перевести на систему событий
		vkQueueWaitIdle(graphicsQueue);

		VkDeviceSize bufferSize = sizeof(itertionMetod->result[0]);
		
		void* data;
		vkMapMemory(device, swapForVertexBuffer->getVkMemoryHandle(), 0, bufferSize, 0, &data);
		//memcpy(data, poligon.data(), (size_t)bufferSize);
		memcpy(data, &(itertionMetod->result.back()), bufferSize);
		vkUnmapMemory(device, swapForVertexBuffer->getVkMemoryHandle());

		vertexBuffer->addBuffer(*swapForVertexBuffer.get());
	}


}

void Render::createCommandBuffers()
{
	commandBuffers.resize(swapChainFramebuffers.size());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers!");
	}

	for (size_t i = 0; i < commandBuffers.size(); i++)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional

		if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = swapChainFramebuffers[i];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapChainExtent;
		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		//Подготовим массивы для передачи данных о вершинах
		std::vector<VkBuffer> rawVertexBuffer;
		std::vector<VkDeviceSize> rawVertexOffsets;

		uint32_t vertexCount = 0;

		vertexCount += static_cast<uint32_t>(vertexBuffer->hotBuffer.getUsingMemorySize() / sizeof(glm::vec3));
		rawVertexBuffer.push_back(vertexBuffer->hotBuffer.getVkBufferHandle());
		rawVertexOffsets.push_back(0);

		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipelines[0].GetPipeline());
		VkBuffer vertexBuffers[] = { vertexBuffer->hotBuffer.getVkBufferHandle() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffers[i], 0, static_cast<uint32_t>(rawVertexBuffer.size()), rawVertexBuffer.data(), rawVertexOffsets.data());
		vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[i], 0, nullptr);
		vkCmdDraw(commandBuffers[i], vertexCount, 1, 0, 0);
		vkCmdEndRenderPass(commandBuffers[i]);

		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record command buffer!");
		}
	}


}

void Render::recreateCommandBuffers(int bufferNumber)
{
	//Очищаем командный буфер
	vkResetCommandBuffer(commandBuffers[bufferNumber], 0);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional

	if (vkBeginCommandBuffer(commandBuffers[bufferNumber], &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass;
	renderPassInfo.framebuffer = swapChainFramebuffers[bufferNumber];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChainExtent;
	VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	//Подготовим массивы для передачи данных о вершинах
	std::vector<VkBuffer> rawVertexBuffer;
	std::vector<VkDeviceSize> rawVertexOffsets;
	uint32_t vertexCount = 0;
	for (auto coldBuffer : vertexBuffer->coldBuffers)
	{
		vertexCount += static_cast<uint32_t>(coldBuffer->getBufferSize() / sizeof(glm::vec3));
		rawVertexBuffer.push_back(coldBuffer->getVkBufferHandle());
		rawVertexOffsets.push_back(0);
	}
	vertexCount += static_cast<uint32_t>(vertexBuffer->hotBuffer.getUsingMemorySize() / sizeof(glm::vec3));
	rawVertexBuffer.push_back(vertexBuffer->hotBuffer.getVkBufferHandle());
	rawVertexOffsets.push_back(0);

	vkCmdBeginRenderPass(commandBuffers[bufferNumber], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(commandBuffers[bufferNumber], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipelines[0].GetPipeline());

	for (auto coldBuffer : vertexBuffer->coldBuffers)
	{
		VkBuffer vertexBuffers[] = { coldBuffer->getVkBufferHandle() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffers[bufferNumber], 0, 1, vertexBuffers, offsets);
		vkCmdBindDescriptorSets(commandBuffers[bufferNumber], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[bufferNumber], 0, nullptr);
		vkCmdDraw(commandBuffers[bufferNumber], static_cast<uint32_t>(coldBuffer->getBufferSize() / sizeof(glm::vec3)), 1, 0, 0);
	}

	VkBuffer vertexBuffers[] = { vertexBuffer->hotBuffer.getVkBufferHandle() };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffers[bufferNumber], 0, 1, vertexBuffers, offsets);
	vkCmdBindDescriptorSets(commandBuffers[bufferNumber], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[bufferNumber], 0, nullptr);
	vkCmdDraw(commandBuffers[bufferNumber], static_cast<uint32_t>(vertexBuffer->hotBuffer.getUsingMemorySize() / sizeof(glm::vec3)), 1, 0, 0);

	//Отрисовываем координатную сетку
	vkCmdBindPipeline(commandBuffers[bufferNumber], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipelines[1].GetPipeline());
	primitive->addComandsToCommandBuffer(commandBuffers[bufferNumber], descriptorSets[bufferNumber], pipelineLayout);

	//закрываем буфер на запись
	vkCmdEndRenderPass(commandBuffers[bufferNumber]);
	
	if (vkEndCommandBuffer(commandBuffers[bufferNumber]) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to record command buffer!");
	}

}
void Render::createSyncObjects()
{
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	imagesInFlight.resize(swapChainImages.size(), VK_NULL_HANDLE);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
	}
}

void Render::drawFrame()
{
	//Ждём ограждение
	vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	//Получаем изображение
	uint32_t imageIndex;
	//3-й параметр задержка в наносекундах UINT64_MAX - отклчает таймер уводя в бесконечность
	VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	//Если во время выполнения получили ошибку, пытаемся её обработать
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		recreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	// Check if a previous frame is using this image (i.e. there is its fence to wait on)
	if (imagesInFlight[imageIndex] != VK_NULL_HANDLE)
	{
		vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}

	// Mark the image as now being in use by this frame
	imagesInFlight[imageIndex] = inFlightFences[currentFrame];
	
	expandVertexBuffer();
	recreateCommandBuffers(imageIndex);
	updateUniformBuffer(imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] }; //Указываем какие семафоры мы хотим подождать
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;

	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] }; //Указываем какие семафоры указывают на конец операции
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(device, 1, &inFlightFences[currentFrame]);

	//Шлём на выполнение
	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	//Ждём семафор
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional
	//Показываем
	result = vkQueuePresentKHR(presentQueue, &presentInfo);
	//Проверяем результат показа
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
	{
		windowManager->setFramebufferResized(false);
		recreateSwapChain();
	}
	else if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to present swap chain image!");
	}
	//Ждём
	//vkQueueWaitIdle(presentQueue);
	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Render::idleDevice()
{
	vkDeviceWaitIdle(device);
}