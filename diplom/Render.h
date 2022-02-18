#pragma once
#include "stdafx.h"
#include "renderHelp/HelpStructures.h"

class DiplomApp;
class WindowManager;
class Camera;

class AbstractItertionMetod;

class Buffer;
class ExpandBufferDeque;
class Primitive;
class StaticPrimitive;
class CurvedPrimitive;

class PipelineInfo;
class GraphicsPipeline;
class DescriptorSetLayout;

class Render
{
private:
	DiplomApp* app; //��������� �� ���� ����������
	WindowManager* windowManager; //��������� �� �������� ����
	AbstractItertionMetod* itertionMetod;
	Camera* camera;

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
	//VkDescriptorSetLayout descriptorSetLayout;
	std::shared_ptr<DescriptorSetLayout> descriptorSetLayout;
	/*�������� � ������ ����������� ��� �������� ������������ ��������*/
	void createGraphicsPipeline();

	//���� ��� �������� ���������� ������������ � ������� ��������� ���������� (����������� � �������)
	VkPipelineLayout pipelineLayout;
	void createPipelineLayout();
	void destroyPipelineLayout();
	//����������� �������
	std::vector<GraphicsPipeline> graphicsPipelines;
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

	std::shared_ptr <StaticPrimitive> primitive;
	std::shared_ptr <CurvedPrimitive> �urvedPrimitive;

	/*�������� � ������ ����������� ��� �������� ������� �������� ������*/
	void createIndexBuffer();
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
	const int MAX_FRAMES_IN_FLIGHT = 3; //������������ ���-�� �������������� �����������
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
	void recreateCommandBuffers(int bufferNumber);
	void createSyncObjects();
	void createPrimitive();

	friend Buffer;
	friend PipelineInfo;
	friend Primitive;
public:
	Render();
	~Render();
	void RenderInit(DiplomApp* new_app, WindowManager* new_windowManager, AbstractItertionMetod* newItertionMetod, Camera* newCamera);
	void drawFrame();
	void idleDevice();

};
