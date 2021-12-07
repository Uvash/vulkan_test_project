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
	PipelineInfo(Render* newRender);
	
	~PipelineInfo();

	void createPipelineInfo();
	VkGraphicsPipelineCreateInfo pipelineInfo;

	Render* render = nullptr;

	renderHelp::ShaderStages shader;
	std::vector<Shader> shaders;

	VkVertexInputBindingDescription bindingDescription{};
	std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions;
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	renderHelp::StaticStage staticStage;
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
};