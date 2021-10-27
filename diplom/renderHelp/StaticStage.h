#pragma once
#include "../stdafx.h"

namespace renderHelp
{
	/*
	* Структура содержит информацию, необходимую для построения графического конвеера.
	* В основном для создания фиксированных стадий
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