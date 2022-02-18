#include "stdafx.h"
#include "CurvedPrimitive.h"
#include "../iteratorMetods/AbstractIterationMetod.h"

CurvedPrimitive::CurvedPrimitive(Render& newRender, size_t newCommandBuffersCount, size_t newTargetPipeline, std::string primitiveName) : Primitive(newRender, newCommandBuffersCount, newTargetPipeline, primitiveName), pBufferDeque{ std::make_shared<ExpandBufferDeque>(newRender) }, swapBuffer{std::make_shared<Buffer>(newRender)}
{
	pBufferDeque->setHotBufferSpecificParameters(VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	pBufferDeque->setColdBufferSpecificParameters(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	pBufferDeque->createBuffer(sizeof(iterationMetod->result[0]), 1024);

	swapBuffer->createBuffer(sizeof(iterationMetod->result[0]), VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

CurvedPrimitive::~CurvedPrimitive()
{

}

void CurvedPrimitive::fillCommandsBuffers()
{

}

void CurvedPrimitive::updateCommandsBuffers(size_t bufferNumber)
{
	//ќчищаем командный буфер
	vkResetCommandBuffer(commandBuffers[bufferNumber], 0);

	VkCommandBufferInheritanceInfo BufferInheritanceInfo{};
	BufferInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
	BufferInheritanceInfo.renderPass = getRenderPass();
	BufferInheritanceInfo.framebuffer = getFramebuffer(bufferNumber);
	BufferInheritanceInfo.occlusionQueryEnable = VK_FALSE; // если что-то заблокирует основной буфер, то будем ждать его разблокировки

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT; // Optional
	beginInfo.pInheritanceInfo = &BufferInheritanceInfo; // Optional

	if (vkBeginCommandBuffer(commandBuffers[bufferNumber], &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = getRenderPass();
	renderPassInfo.framebuffer = getFramebuffer(bufferNumber);
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = getExtent2D();
	VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	//vkCmdBeginRenderPass(commandBuffers[bufferNumber], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(commandBuffers[bufferNumber], VK_PIPELINE_BIND_POINT_GRAPHICS, getGraphicsPipeline(targetPipeline));

	for (auto coldBuffer : pBufferDeque->coldBuffers)
	{
		VkBuffer vertexBuffers[] = { coldBuffer->getVkBufferHandle() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffers[bufferNumber], 0, 1, vertexBuffers, offsets);
		vkCmdBindDescriptorSets(commandBuffers[bufferNumber], VK_PIPELINE_BIND_POINT_GRAPHICS, getPipelineLayout(), 0, 1, &getDescriptorSet(bufferNumber), 0, nullptr);
		//vkCmdDrawIndirect();
		vkCmdDraw(commandBuffers[bufferNumber], static_cast<uint32_t>(coldBuffer->getBufferSize() / sizeof(glm::vec3)), 1, 0, 0);
	}

	VkBuffer vertexBuffers[] = { pBufferDeque->hotBuffer.getVkBufferHandle() };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffers[bufferNumber], 0, 1, vertexBuffers, offsets);
	vkCmdBindDescriptorSets(commandBuffers[bufferNumber], VK_PIPELINE_BIND_POINT_GRAPHICS, getPipelineLayout(), 0, 1, &getDescriptorSet(bufferNumber), 0, nullptr);
	vkCmdDraw(commandBuffers[bufferNumber], static_cast<uint32_t>(pBufferDeque->hotBuffer.getUsingMemorySize() / sizeof(glm::vec3)), 1, 0, 0);

	//закрываем буфер на запись
	//vkCmdEndRenderPass(commandBuffers[bufferNumber]);

	if (vkEndCommandBuffer(commandBuffers[bufferNumber]) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to record command buffer!");
	}
}

void CurvedPrimitive::updateInformation()
{
	if (iterationMetod->needUpdate())
	{
		// ќ—“џЋ№ ѕеревести на систему событий
		vkQueueWaitIdle(getGraphicsQueue());

		VkDeviceSize bufferSize = sizeof(iterationMetod->result[0]);

		void* data;
		vkMapMemory(getDevice(), swapBuffer->getVkMemoryHandle(), 0, bufferSize, 0, &data);
		//memcpy(data, poligon.data(), (size_t)bufferSize);
		memcpy(data, &(iterationMetod->result.back()), bufferSize);
		vkUnmapMemory(getDevice(), swapBuffer->getVkMemoryHandle());

		pBufferDeque->addBuffer(*swapBuffer.get());
	}
}

void CurvedPrimitive::connectToItertionMetod(AbstractItertionMetod* metod)
{
	if (metod == nullptr)
	{
		throw std::runtime_error("CurvedPrimitive: AbstractItertionMetod has nullptr!");
	}

	iterationMetod = metod;
}