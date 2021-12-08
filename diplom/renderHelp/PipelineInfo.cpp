#include "stdafx.h"
#include "PipelineInfo.h"
#include "..\Render.h"

PipelineInfo::PipelineInfo(Render& newRender) : render{ newRender }, pipelineInfo{}, bindingDescription{ Vertex::getBindingDescription() }, \
attributeDescriptions{ Vertex::getAttributeDescriptions() }, inputAssembly{}, staticStage{&newRender.swapChainExtent}
{

}

PipelineInfo::~PipelineInfo()
{

}

const VkGraphicsPipelineCreateInfo& PipelineInfo::getPipelineInfo() &
{
	return pipelineInfo;
}

void  PipelineInfo::assemblePipelineInfo()
{
	createShaders();
	createVertexInputInfo();
	createInputAssembly();
	createPipelineInfo();
}

void PipelineInfo::createPipelineInfo()
{
	//Начинаем описывать структуру описывающую параметры конвера
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	//Включаем шейдеры
	pipelineInfo.stageCount = shader->shaderStages.size();
	pipelineInfo.pStages = shader->shaderStages.data();

	//Включаем описание фиксированных стадий
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &staticStage.viewportState;
	pipelineInfo.pRasterizationState = &staticStage.rasterizer;
	pipelineInfo.pMultisampleState = &staticStage.multisampling;
	pipelineInfo.pDepthStencilState = nullptr; // Optional
	pipelineInfo.pColorBlendState = &staticStage.colorBlending;
	pipelineInfo.pDynamicState = nullptr; // Optional
	//Подключаем информацию глобальных переменных
	pipelineInfo.layout = render.pipelineLayout;
	//Подключаем проходы рендеренига
	pipelineInfo.renderPass = render.renderPass;
	pipelineInfo.subpass = 0;
	//Указываем преведущие конвееры (их нет)
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional
}

void PipelineInfo::createShaders()
{
	shader = std::make_shared<renderHelp::ShaderStages>(render.device);
}

void PipelineInfo::createVertexInputInfo()
{
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
}

void PipelineInfo::createInputAssembly()
{
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
	inputAssembly.primitiveRestartEnable = VK_FALSE;
}