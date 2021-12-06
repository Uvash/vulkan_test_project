#pragma once
#include "../stdafx.h"
#include "HelpStructures.h"
#include "ShaderLoader.h"
#include "StaticStage.h"
/*
* ����� PipelineInfo
* ���������� � ������ VkGraphicsPipelineCreateInfo � ��� �� ��������������� ��������� � ������
*/
class Render;

class PipelineInfo
{
public:
	PipelineInfo() = delete;
	PipelineInfo(Render* newRender);
	
	~PipelineInfo();

	void createPipelineInfo();
	VkGraphicsPipelineCreateInfo pipelineInfo;

	Render* render = nullptr;

	renderHelp::ShaderStages shader;
	VkVertexInputBindingDescription bindingDescription{};
	std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions;
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	renderHelp::StaticStage staticStage;
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
};