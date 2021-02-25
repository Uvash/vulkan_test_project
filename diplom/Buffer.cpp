#include "stdafx.h"

#include "Buffer.h"
#include "Render.h"

Buffer::Buffer()
{

}

Buffer::~Buffer()
{
	if (init_complite && buffer_complite)
	{
		vkDestroyBuffer(render->device, vkBuffer, nullptr);
		vkFreeMemory(render->device, bufferMemory, nullptr);
		buffer_complite = false;
	}

}

void Buffer::initBuffer(Render* new_render)
{
	render = new_render;
	init_complite = true;
}

/*
 *@brief вспомогательная функция для создания буффера и выделения для него памяти
 *
 *@param size размер буффера в байтах
 *@param usage флаги описывающие с чем может работать буффер
 *@param properties дополнительные параметры памяти под буффер
 *@param buffer хендл буффера
 *@param bufferMemory хендл под выделяемую память
 *@return создаёт буффер и выделяет под него память
 */
void Buffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
{
	bufferSize = size;
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	//Создаём буффер
	if (vkCreateBuffer(render->device, &bufferInfo, nullptr, &vkBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(render->device, vkBuffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);
	//Выделяем память
	if (vkAllocateMemory(render->device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate buffer memory!");
	}

	vkBindBufferMemory(render->device, vkBuffer, bufferMemory, 0);

	buffer_complite = true;
}

/*
 *@brief Определяет наиболее оптимальный тип памяти для наших запросов
 *
 *@param typeFilter тип необходимой памяти
 *@param properties дополнительные параметры этой памяти
 *@return оптимальный тип памяти
 */
uint32_t Buffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(render->physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) &&
			(memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}
	throw std::runtime_error("failed to find suitable memory type!");
}

/*
 *@brief Функция копирования буфферов
 *
 *@param srcBuffer	буффер источник
 *@param dstBuffer	буффер приёмник
 *@param size		размер копируемой области
 *@return
 */
void  Buffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	//Создаём командный буффер
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = render->transfertСommandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(render->device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; //Предупреждаем что будем пользовать один раз
	//Записываем в него команду копирования
	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0; // Optional
	copyRegion.dstOffset = 0; // Optional
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	vkEndCommandBuffer(commandBuffer);

	//Отправляем на выполнение
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;


	vkQueueSubmit(render->transfertQueue, 1, &submitInfo, VK_NULL_HANDLE);
	//Дожидаемся выполнения
	vkQueueWaitIdle(render->transfertQueue);

	vkFreeCommandBuffers(render->device, render->transfertСommandPool, 1, &commandBuffer);
}
