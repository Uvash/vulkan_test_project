#include "stdafx.h"
#include "Render.h"
#include "DiplomApp.h"
#include "WindowManager.h"
#include "HelpStructures.h"

Render::Render()
{

}
Render::~Render()
{
	cleanupSwapChain();
	vkDestroyDevice(device, nullptr);
	if (app->enableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}
	vkDestroySurfaceKHR(instance, surface, nullptr); //Уничтожаем поверхность вывода
	vkDestroyInstance(instance, nullptr); //Уничтожаем экземпляр вулкана
}

void Render::RenderInit(DiplomApp* new_app, WindowManager* new_windowManager)
{
	if (new_app == nullptr)
	{
		throw std::runtime_error("App must be created before create Vulkan");
	}
	if (new_windowManager == nullptr)
	{
		throw std::runtime_error("WindowManager must be created before create Vulkan");
	}
	app = new_app;
	windowManager = new_windowManager;

	createInstance();
	createSurface();
	setupDebugMessenger();
	pickPhysicalDevice();
	createLogicalDevice();
	createSwapChain();
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
	/*
	//Получаем данные об устройстве
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	//Получаем данные об поддерживаемых фичах
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	std::cout << deviceProperties.deviceName << '\n';
	*/
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

void Render::cleanupSwapChain()
{
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
		VkExtent2D actualExtent =
		{
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};
	}
}