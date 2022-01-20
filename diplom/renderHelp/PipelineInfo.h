#pragma once
#include "../stdafx.h"
#include "HelpStructures.h"
#include "ShaderLoader.h"
#include "StaticStage.h"
#include "Shader.h"
/*
* ����� PipelineInfo
* ���������� � ������ VkGraphicsPipelineCreateInfo � ��� �� ��������������� ��������� � ������
*/
class Render;

class PipelineInfo
{
public:
	PipelineInfo() = delete;
	PipelineInfo(Render& newRender);
	
	virtual ~PipelineInfo();

	virtual void assemblePipelineInfo(); //� ��������� � ������������ �� �������� ����� ����������� ������� ��� ��������� �� pvc

	const VkGraphicsPipelineCreateInfo& getPipelineInfo() &;
	

protected:
	virtual void setAttributeDescription();
	virtual void createPipelineInfo();
	virtual void createShaders(int shaderType = renderHelp::shaderType::STATIC_COLOR);
	virtual void createVertexInputInfo();
	virtual void createInputAssembly();

	Render& render;
	std::shared_ptr<renderHelp::ShaderStages> shader;
	VkVertexInputBindingDescription bindingDescription{};
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	renderHelp::StaticStage staticStage;
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	VkGraphicsPipelineCreateInfo pipelineInfo;
};