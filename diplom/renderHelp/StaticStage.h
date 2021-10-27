#pragma once
#include "../stdafx.h"

namespace renderHelp
{
	/*
	* ��������� �������� ����������, ����������� ��� ���������� ������������ ��������.
	* � �������� ��� �������� ������������� ������
	*/
	struct StaticStage
	{
		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineViewportStateCreateInfo viewportState;
		VkPipelineRasterizationStateCreateInfo rasterizer;
		VkPipelineMultisampleStateCreateInfo multisampling;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlending;

		StaticStage(VkExtent2D * swapChainExtent);
	};
}