#include "stdafx.h"
#include "Primitive.h"
#include "../renderHelp/HelpStructures.h"
#include "../Buffer.h"

Primitive::Primitive(Render& newRender, size_t newCommandBuffersCount, size_t newTargetPipeline, std::string primitiveName) : render (newRender), targetPipeline(newTargetPipeline), commandBuffersCount(newCommandBuffersCount)
{
	createCommandsBuffers();
}

 Primitive::~Primitive()
{
	 clearCommandsBuffers();
}
/*
void Primitive::addComandsToCommandBuffer(VkCommandBuffer& commandBuffer, VkDescriptorSet& descriptionSet, VkPipelineLayout& pipelineLayout)
{
	VkDeviceSize offset =  0;
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &pBuffer->getVkBufferHandle(), &offset);

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptionSet, 0, nullptr);
	vkCmdDraw(commandBuffer, static_cast<uint32_t>(pBuffer->getBufferSize() / sizeof(VertexColored)), 1, 0, 0);
}
*/
 void Primitive::createCommandsBuffers()
 {
	 commandBuffers.resize(commandBuffersCount);
	 allocCommandBuffers(commandBuffersCount);
 }

 void Primitive::clearCommandsBuffers()
 {
	 if (commandBuffers.size() != 0)
	 {
		 vkFreeCommandBuffers(render.device, render.commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
		 commandBuffers.clear();
	 }
	 
 }

void Primitive::fillCommandsBuffers()
{
	return;
}

void Primitive::updateCommandsBuffers(size_t bufferNumber)
{
	return;
}

void Primitive::allocCommandBuffers(size_t newCommandBuffersCount)
{
	commandBuffers.resize(newCommandBuffersCount);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = render.commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	if (vkAllocateCommandBuffers(render.device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("Primitive: failed to allocate command buffers!");
	}
}

VkCommandBuffer& Primitive::getCommandBuffer(size_t bufferNumber)
{
	if (bufferNumber >= commandBuffers.size())
	{
		throw std::runtime_error("Primitive: failed to get command buffers!");
	}
	return commandBuffers[bufferNumber];
}

