#pragma once
#include "../stdafx.h"

/*
* ����� ����������� �������
* ������ ��� ��������������� ������ � �������� ���������� �� ����������� ������ �������
* �������� ������� � ���������� ����� - ��������
*/
class GraphicsPipeline
{
public:
	GraphicsPipeline();
	GraphicsPipeline(VkDevice curentDevice, VkGraphicsPipelineCreateInfo const* pipelineInfo);
	~GraphicsPipeline();
	//��������� ��������� ����������� 
	GraphicsPipeline(GraphicsPipeline const & other) = delete;
	GraphicsPipeline& operator= (GraphicsPipeline const& other) = delete; //����� �� �������� ������ �� lvalue � ��� ������. �� �� ��� �������� ��������� ����
	//���������� ��������� �����������
	
	GraphicsPipeline(GraphicsPipeline&& that); //����� �� ���������� ������ �� rvalue, �������� ����� �������, � ��� ������
	GraphicsPipeline& operator=(GraphicsPipeline&& that); 	

	friend void swap(GraphicsPipeline& lhs, GraphicsPipeline& rhs) noexcept
	{
		std::swap(lhs.graphicsPipeline, rhs.graphicsPipeline);
		std::swap(lhs.device, rhs.device);
	}

	VkPipeline& GetPipeline();
private:
	VkPipeline graphicsPipeline = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;

};