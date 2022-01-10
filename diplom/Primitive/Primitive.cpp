#include "stdafx.h"
#include "Primitive.h"
#include "../Buffer.h"

Primitive::Primitive(Render& newRender) : pBuffer{ std::make_shared<Buffer>(newRender) }
{

}

void Primitive::addComandsToCommandBuffer(VkCommandBuffer& commandBuffer, VkDescriptorSet& descriptionSet, VkPipelineLayout& pipelineLayout)
{
	VkDeviceSize offset =  0;
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &pBuffer->getVkBufferHandle(), &offset);

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptionSet, 0, nullptr);
	vkCmdDraw(commandBuffer, static_cast<uint32_t>(pBuffer->getBufferSize() / sizeof(glm::vec3)), 1, 0, 0);
}