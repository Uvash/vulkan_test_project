#pragma once
#include "stdafx.h"

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily; //����� ������� ��� ���������
	std::optional<uint32_t> presentFamily;	//����� ������� ��� ������
	std::optional<uint32_t> transferFamily;	//����� ������� ��� ���������� �������

	//��� �� ������� ����������������	
	bool isComplete()
	{
		return	graphicsFamily.has_value() &&
			presentFamily.has_value() &&
			transferFamily.has_value();
	}
};

//��������� ����������� �������� ������� ������
struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;

	//��������� ����������� ������� ������ ����� ������� � � ����� ���������
	static VkVertexInputBindingDescription getBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0; //���������� � ��� � ������ ������ ���������
		bindingDescription.stride = sizeof(Vertex); //�������� �� ��������� ���������
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}
	//��������� ����������� � ��� ����� ������ ������ �����
	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
		//��������� ��������� ��� ���������
		attributeDescriptions[0].binding = 0;	//�������� �� ������ ���������
		attributeDescriptions[0].location = 0;	//� ������ ���� �� ����� ����� ������
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT; //��������� ����� ��� ������ ������� vec2
		attributeDescriptions[0].offset = offsetof(Vertex, pos); //��������� �������� �� ������ ������
		//��������� ��������� ��� �����
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}
};