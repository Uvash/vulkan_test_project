#include "stdafx.h"
#include "PipelineInfo.h"
#include "..\Render.h"

PipelineInfo::PipelineInfo(Render* newRender) : render{ newRender }, pipelineInfo{}, shader{ newRender->device }, bindingDescription{ Vertex::getBindingDescription() }, \
attributeDescriptions{ Vertex::getAttributeDescriptions() }, inputAssembly{}, staticStage{&newRender->swapChainExtent}
{
	if (render == nullptr)
	{
		throw std::runtime_error("PipelineInf: pointer Render is nullptr");
	}
}

void PipelineInfo::createPipelineInfo()
{
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	//�������� ��������� ��������� ����������� ��������� �������
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	//�������� �������
	pipelineInfo.stageCount = shader.shaderStages.size();
	pipelineInfo.pStages = shader.shaderStages.data();

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
	pipelineInfo.layout = render->pipelineLayout;
	//���������� ������� �����������
	pipelineInfo.renderPass = render->renderPass;
	pipelineInfo.subpass = 0;
	//��������� ���������� �������� (�� ���)
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional
}

PipelineInfo::~PipelineInfo()
{

}