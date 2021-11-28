#include "stdafx.h"
#include "GraphicsPipeline.h"

GraphicsPipeline::GraphicsPipeline()
{

}

GraphicsPipeline::GraphicsPipeline(VkDevice curentDevice, VkGraphicsPipelineCreateInfo const *pipelineInfo)
{
	if (curentDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("GraphicsPipeline: Device handler is null");
	}
	
	device = curentDevice;

	if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
	{
		throw std::runtime_error("GraphicsPipeline: failed to create graphics pipeline!");
	}
}
GraphicsPipeline::~GraphicsPipeline()
{
	if(graphicsPipeline != VK_NULL_HANDLE && device != VK_NULL_HANDLE)
		vkDestroyPipeline(device, graphicsPipeline, nullptr);
}

GraphicsPipeline::GraphicsPipeline(GraphicsPipeline&& that) : graphicsPipeline(VK_NULL_HANDLE), device(VK_NULL_HANDLE)
{
	swap(*this, that);
}

VkPipeline& GraphicsPipeline::GetPipeline()
{
	return graphicsPipeline;
}
