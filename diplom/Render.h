#pragma once
#include "stdafx.h"
#include "renderHelp/HelpStructures.h"

class DiplomApp;
class WindowManager;
class Buffer;
class ExpandBufferDeque;
class AbstractItertionMetod;
class Camera;
class GraphicsPipeline;
class DescriptorSetLayout;

class Render
{
private:
	DiplomApp* app; //Указатель на наше приложение
	WindowManager* windowManager; //Указатель на менеджер окон
	AbstractItertionMetod* itertionMetod;
	Camera* camera;

	/*АТРИБУТЫ И МЕТОДЫ НЕОБХОДИМЫЕ ДЛЯ СОЗДАНИЯ ОБЪЕКТА ВУЛКАН*/
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

	/*АТРИБУТЫ И МЕТОДЫ НЕОБХОДИМЫЕ ДЛЯ СОЗДАНИЯ ОБЪЕКТА ПОВЕРХНОСТЬ*/
	//Поверхность на которую мы будем выводить изображение
	VkSurfaceKHR surface;
	void createSurface();
	/*Подключаем обработчик ошибок*/
	//экземпляр обработчика ошибок
	VkDebugUtilsMessengerEXT debugMessenger;
	void setupDebugMessenger();
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
	/*АТРИБУТЫ И МЕТОДЫ НЕОБХОДИМЫЕ ДЛЯ СОЗДАНИЯ ФИЗИЧЕСКОГО УСТРОЙСТВА*/
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
	/*АТРИБУТЫ И МЕТОДЫ НЕОБХОДИМЫЕ ДЛЯ СОЗДАНИЯ ЛОГИЧЕСКОГО УСТРОЙСТВА*/
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
	/*АТРИБУТЫ И МЕТОДЫ НЕОБХОДИМЫЕ ДЛЯ СОЗДАНИЯ ЦЕПОЧКИ ОБМЕНА*/
	void createSwapChain();
	void recreateSwapChain();
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
	/*АТРИБУТЫ И МЕТОДЫ НЕОБХОДИМЫЕ ДЛЯ СОЗДАНИЯ ПРЕДСТАВЛЕНИЙ ИЗОБРАЖЕНИЙ*/
	void createImageViews();
	//Представления изображений
	std::vector<VkImageView> swapChainImageViews;
	/*АТРИБУТЫ И МЕТОДЫ НЕОБХОДИМЫЕ ДЛЯ СОЗДАНИЯ ПРОХОДА РЕНДЕРИНГА*/
	void createRenderPass();
	//Проход рендеринга
	VkRenderPass renderPass;
	/*АТРИБУТЫ И МЕТОДЫ НЕОБХОДИМЫЕ ДЛЯ СОЗДАНИЯ СЛОЁВ (описывает как и какую информацию читать шейдерам)*/
	void createDescriptorSetLayout();
	//Дискриптор указывающий на глобальные переменные для использования в шейдере
	//VkDescriptorSetLayout descriptorSetLayout;
	std::shared_ptr<DescriptorSetLayout> descriptorSetLayout;
	/*АТРИБУТЫ И МЕТОДЫ НЕОБХОДИМЫЕ ДЛЯ СОЗДАНИЯ ГРАФИЧЕСКОГО КОНВЕЕРА*/
	void createGraphicsPipeline();

	const std::vector<Vertex> vertices = {
		{{-0.5f, -0.5f, 0.5f}},
		{{0.5f, -0.5f, 0.5f}},
		{{0.5f, 0.5f, 0.5f}},
		{{-0.5f, 0.5f, 0.5f}},
		{{-0.5f, -0.5f, -0.5f}},
		{{0.5f, -0.5f, -0.5f}},
		{{0.5f, 0.5f, -0.5f}},
		{{-0.5f, 0.5f, -0.5f}}
	};

	//Слой для хранения переменных используемых в шейдере локальных переменных (привязанных к вершине)
	VkPipelineLayout pipelineLayout;
	//Графический конвеер
	std::vector<GraphicsPipeline> graphicsPipelines;
	VkPipeline graphicsPipeline;
	/*АТРИБУТЫ И МЕТОДЫ НЕОБХОДИМЫЕ ДЛЯ СОЗДАНИЯ БУФФЕРОВ ИЗОБРАЖЕНИЯ*/
	void createFramebuffers();
	//Кадровые буфферы
	std::vector<VkFramebuffer> swapChainFramebuffers;
	/*АТРИБУТЫ И МЕТОДЫ НЕОБХОДИМЫЕ ДЛЯ СОЗДАНИЯ КОМАНДНОГО ПУЛА*/
	//Командный пул
	VkCommandPool commandPool;
	//Командный пул для работы в памяти
	VkCommandPool transfertСommandPool;
	void initCommandPool();
	void createCommandPool(uint32_t familyIndex, VkCommandPool* pool);
	/*АТРИБУТЫ И МЕТОДЫ НЕОБХОДИМЫЕ ДЛЯ СОЗДАНИЯ БУФФЕРА ВЕРШИН*/
	void createVertexBuffer();
	//Буфер вершин
	std::shared_ptr<ExpandBufferDeque> vertexBuffer;
	/*АТРИБУТЫ И МЕТОДЫ НЕОБХОДИМЫЕ ДЛЯ СОЗДАНИЯ БУФФЕРА ИНДЕКСОВ ВЕРШИН*/
	void createIndexBuffer();
	std::shared_ptr<Buffer> swapForVertexBuffer;
	/*АТРИБУТЫ И МЕТОДЫ НЕОБХОДИМЫЕ ДЛЯ СОЗДАНИЯ БУФФЕРА ГЛОБАЛЬНЫХ ВЕРШИН*/
	//Буффер для глобальных переменных
	std::vector<std::shared_ptr<Buffer>> uniformBuffers;

	void createUniformBuffers();
	void updateUniformBuffer(uint32_t currentImage);
	/*АТРИБУТЫ И МЕТОДЫ НЕОБХОДИМЫЕ ДЛЯ СОЗДАНИЯ ПУЛА ДЕСКРИПТОРА*/
	void createDescriptorPool();
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
	
	void createDescriptorSets();
	void expandVertexBuffer();
	/*АТРИБУТЫ И МЕТОДЫ НЕОБХОДИМЫЕ ДЛЯ СОЗДАНИЯ КОМАНДНОГО БУФФЕРА*/
	//Командный буфер
	std::vector<VkCommandBuffer> commandBuffers;
	const int MAX_FRAMES_IN_FLIGHT = 3; //Максимальное кол-во обрабатываемых изображений
	//Семафоры сигнализирующий о готовности для рендеринга
	std::vector<VkSemaphore> imageAvailableSemaphores;
	//Семафоры сигнализирует о завершении рендеринга
	std::vector<VkSemaphore> renderFinishedSemaphores;
	//Какой кадр из буфера рисуется
	size_t currentFrame = 0;
	//Заборы для для предотвращения показа второго кадра
	std::vector<VkFence> inFlightFences;
	//Забор для предотвращения рендеринга кадра который рендериться
	std::vector<VkFence> imagesInFlight;

	void createCommandBuffers();
	void recreateCommandBuffers(int bufferNumber);
	void createSyncObjects();

	friend Buffer;
public:
	Render();
	~Render();
	void RenderInit(DiplomApp* new_app, WindowManager* new_windowManager, AbstractItertionMetod* newItertionMetod, Camera* newCamera);
	void drawFrame();
	void idleDevice();

};
