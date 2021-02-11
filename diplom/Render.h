#pragma once
#include "stdafx.h"

class DiplomApp;

class Render
{
private:
	DiplomApp* app; //Указатель на наше приложение

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


public:
	Render();
	~Render();
	void RenderInit(DiplomApp* new_app);
};
