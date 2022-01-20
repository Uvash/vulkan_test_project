#pragma once
#include "stdafx.h"

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily; //Номер очереди для отрисовки
	std::optional<uint32_t> presentFamily;	//Номер очереди для показа
	std::optional<uint32_t> transferFamily;	//Номер очереди для управления памятью

	//Все ли очереди инициализированы	
	bool isComplete()
	{
		return	graphicsFamily.has_value() &&
			presentFamily.has_value() &&
			transferFamily.has_value();
	}
};

//Струкутра описывающая свойства цепочки обмена
struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct Vertex {
	glm::vec3 pos;

	//Структура описывающая вулкану откуда брать вершины и с каким смещением
	static VkVertexInputBindingDescription getBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0; //Координаты у нас с самого начала структуры
		bindingDescription.stride = sizeof(Vertex); //Смещение до следующей структуры
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}
	//Структура описывающая о том какие данные откуда брать
	static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{1};
		//Указываем параметры для координат
		attributeDescriptions[0].binding = 0;	//Смещение от начала структуры
		attributeDescriptions[0].location = 0;	//С какого слоя мы будет брать данные
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT; //Указываем какой тип данных передаём vec2
		attributeDescriptions[0].offset = offsetof(Vertex, pos); //Указываем смещение до списка вершин
		return attributeDescriptions;
	}

};

struct VertexColored {
	glm::vec3 pos;
	glm::vec3 color;

	//Структура описывающая вулкану откуда брать вершины и с каким смещением
	static VkVertexInputBindingDescription getBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0; //Координаты у нас с самого начала структуры
		bindingDescription.stride = sizeof(VertexColored); //Смещение до следующей структуры
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}
	//Структура описывающая о том какие данные откуда брать
	static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{ 2 };
		//Указываем параметры для координат
		attributeDescriptions[0].binding = 0;	//Смещение от начала структуры
		attributeDescriptions[0].location = 0;	//С какого слоя мы будет брать данные
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT; //Указываем какой тип данных передаём vec2
		attributeDescriptions[0].offset = offsetof(VertexColored, pos); //Указываем смещение до списка вершин
		
		//Указываем параметры для цвета
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(VertexColored, color);

		return attributeDescriptions;
	}

};

//Структура содержащия глобальные матрицы преобразований
struct UniformBufferObject
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};
