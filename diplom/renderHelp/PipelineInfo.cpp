#include "stdafx.h"
#include "PipelineInfo.h"
#include "..\Render.h"

PipelineInfo::PipelineInfo(Render& newRender) : render{ newRender }, pipelineInfo{}, \
 inputAssembly{}, staticStage{&newRender.swapChainExtent}
{

}

PipelineInfo::~PipelineInfo()
{

}

void  PipelineInfo::assemblePipelineInfo()
{
	setAttributeDescription();
	createShaders();
	createVertexInputInfo();
	createInputAssembly();
	createPipelineInfo();
}

const VkGraphicsPipelineCreateInfo& PipelineInfo::getPipelineInfo() &
{
	return pipelineInfo;
}

void PipelineInfo::setAttributeDescription()
{
	bindingDescription = Vertex::getBindingDescription();
	attributeDescriptions = Vertex::getAttributeDescriptions();
}

void PipelineInfo::createPipelineInfo()
{
	//�������� ��������� ��������� ����������� ��������� �������
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	//�������� �������
	pipelineInfo.stageCount = shader->shaderStages.size();
	pipelineInfo.pStages = shader->shaderStages.data();

	//�������� �������� ������������� ������
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &staticStage.viewportState;
	pipelineInfo.pRasterizationState = &staticStage.rasterizer;
	pipelineInfo.pMultisampleState = &staticStage.multisampling;
	pipelineInfo.pDepthStencilState = nullptr; // Optional
	pipelineInfo.pColorBlendState = &staticStage.colorBlending;
	pipelineInfo.pDynamicState = nullptr; // Optional
	//���������� ���������� ���������� ����������
	pipelineInfo.layout = render.pipelineLayout;
	//���������� ������� �����������
	pipelineInfo.renderPass = render.renderPass;
	pipelineInfo.subpass = 0;
	//��������� ���������� �������� (�� ���)
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional
}

void PipelineInfo::createShaders(int shaderType)
{
	shader = std::make_shared<renderHelp::ShaderStages>(render.device);
	shader->loadShaders(shaderType);
}

void PipelineInfo::createVertexInputInfo()
{
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;

	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
}

void PipelineInfo::createInputAssembly()
{
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
	inputAssembly.primitiveRestartEnable = VK_FALSE;
}