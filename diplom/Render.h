#pragma once
#include "stdafx.h"
#include "HelpStructures.h"

class DiplomApp;
class WindowManager;
class Buffer;

class Render
{
private:
	DiplomApp* app; //��������� �� ���� ����������
	WindowManager* windowManager; //��������� �� �������� ����

	/*�������� � ������ ����������� ��� �������� ������� ������*/
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

	/*�������� � ������ ����������� ��� �������� ������� �����������*/
	//����������� �� ������� �� ����� �������� �����������
	VkSurfaceKHR surface;
	void createSurface();
	/*���������� ���������� ������*/
	//��������� ����������� ������
	VkDebugUtilsMessengerEXT debugMessenger;
	void setupDebugMessenger();
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
	/*�������� � ������ ����������� ��� �������� ����������� ����������*/
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
	/*�������� � ������ ����������� ��� �������� ����������� ����������*/
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
	/*�������� � ������ ����������� ��� �������� ������� ������*/
	void createSwapChain();
	void recreateSwapChain();
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
	/*�������� � ������ ����������� ��� �������� ������������� �����������*/
	void createImageViews();
	//������������� �����������
	std::vector<VkImageView> swapChainImageViews;
	/*�������� � ������ ����������� ��� �������� ������� ����������*/
	void createRenderPass();
	//������ ����������
	VkRenderPass renderPass;
	/*�������� � ������ ����������� ��� �������� ��Ψ� (��������� ��� � ����� ���������� ������ ��������)*/
	void createDescriptorSetLayout();
	//���������� ����������� �� ���������� ���������� ��� ������������� � �������
	VkDescriptorSetLayout descriptorSetLayout;
	/*�������� � ������ ����������� ��� �������� ������������ ��������*/
	void createGraphicsPipeline();
	static std::vector<char> readFile(const std::string& filename);
	VkShaderModule createShaderModule(const std::vector<char>& code);

	const std::vector<Vertex> vertices = {
	{ {-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 0.0f}},
	{ {0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
	{ {0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}},
	{ {-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}},
	{ {-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{ {0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
	{ {0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 1.0f}},
	{ {-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}}
	};

	//���� ��� �������� ���������� ������������ � ������� ��������� ���������� (����������� � �������)
	VkPipelineLayout pipelineLayout;
	//����������� �������
	VkPipeline graphicsPipeline;
	/*�������� � ������ ����������� ��� �������� �������� �����������*/
	void createFramebuffers();
	//�������� �������
	std::vector<VkFramebuffer> swapChainFramebuffers;
	/*�������� � ������ ����������� ��� �������� ���������� ����*/
	//��������� ���
	VkCommandPool commandPool;
	//��������� ��� ��� ������ � ������
	VkCommandPool transfert�ommandPool;
	void initCommandPool();
	void createCommandPool(uint32_t familyIndex, VkCommandPool* pool);
	/*�������� � ������ ����������� ��� �������� ������� ������*/
	void createVertexBuffer();
	//����� ������
	std::shared_ptr<Buffer> vertexBuffer;
	/*�������� � ������ ����������� ��� �������� ������� �������� ������*/
	void createIndexBuffer();
	//��������� uint16_t �� ����� ��������� ���� 65535 ������
	const std::vector<uint16_t> indices = { 0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 0, 3, 7, 7, 4, 0, 3, 2, 6, 6, 7, 3, 2, 1, 5, 5, 6, 2, 0, 4, 5, 5, 1, 0 };
	//������ �������� ������
	std::shared_ptr<Buffer> indexBuffer;
	/*�������� � ������ ����������� ��� �������� ������� ���������� ������*/
	//������ ��� ���������� ����������
	std::vector<std::shared_ptr<Buffer>> uniformBuffers;

	void createUniformBuffers();
	void updateUniformBuffer(uint32_t currentImage);
	/*�������� � ������ ����������� ��� �������� ���� �����������*/
	void createDescriptorPool();
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
	
	void createDescriptorSets();
	/*�������� � ������ ����������� ��� �������� ���������� �������*/
	//��������� �����
	std::vector<VkCommandBuffer> commandBuffers;
	const int MAX_FRAMES_IN_FLIGHT = 2; //������������ ���-�� �������������� �����������
	//�������� ��������������� � ���������� ��� ����������
	std::vector<VkSemaphore> imageAvailableSemaphores;
	//�������� ������������� � ���������� ����������
	std::vector<VkSemaphore> renderFinishedSemaphores;
	//����� ���� �� ������ ��������
	size_t currentFrame = 0;
	//������ ��� ��� �������������� ������ ������� �����
	std::vector<VkFence> inFlightFences;
	//����� ��� �������������� ���������� ����� ������� �����������
	std::vector<VkFence> imagesInFlight;

	void createCommandBuffers();
	void createSyncObjects();

	friend Buffer;
public:
	Render();
	~Render();
	void RenderInit(DiplomApp* new_app, WindowManager* new_windowManager);
	void drawFrame();
	void idleDevice();

};
