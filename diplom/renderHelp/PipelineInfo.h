#pragma once
#include "../stdafx.h"
#include "HelpStructures.h"
#include "ShaderLoader.h"
#include "StaticStage.h"
#include "Shader.h"
/*
* Класс PipelineInfo
* Генерирует и хранит VkGraphicsPipelineCreateInfo а так же вспомогательные структуры и классы
*/
class Render;

class PipelineInfo
{
public:
	PipelineInfo() = delete;
	PipelineInfo(Render& newRender);
	
	virtual ~PipelineInfo();

	void assemblePipelineInfo(); //К сожалению в конструкторе не возможен вызов виртуальных методов без натыкания на pvc
	const VkGraphicsPipelineCreateInfo& getPipelineInfo() &;
	

protected:
	virtual void createPipelineInfo();
	virtual void createShaders();
	virtual void createVertexInputInfo();
	virtual void createInputAssembly();

	Render& render;
	std::shared_ptr<renderHelp::ShaderStages> shader;
	VkVertexInputBindingDescription bindingDescription{};
	std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions;
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	renderHelp::StaticStage staticStage;
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	VkGraphicsPipelineCreateInfo pipelineInfo;
};