#include "stdafx.h"
#include "Render.h"
#include "DiplomApp.h"

Render::Render()
{

}
Render::~Render()
{
	vkDestroyInstance(instance, nullptr); //Уничтожаем экземпляр вулкана
}

void Render::RenderInit(DiplomApp* new_app)
{
	if (new_app == nullptr)
	{
		throw std::runtime_error("App must be created before create Vulkan");
	}
	app = new_app;
	createInstance();
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
