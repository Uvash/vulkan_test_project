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
		deallocBuffer();
		deallocMemory();

	}

}

void Buffer::initBuffer(Render* new_render)
{
	render = new_render;
	init_complite = true;
}

/*
 *@brief ��������������� ������� ��� �������� ������� � ��������� ��� ���� ������
 *
 *@param size ������ ������� � ������
 *@param usage ����� ����������� � ��� ����� �������� ������
 *@param properties ����� ������ ��� ������
 *@return ������ ������ � �������� ��� ���� ������
 */
void Buffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
{
	allocBuffer(size, usage);
	allocMemory(size, properties);
	bindMemory();
}

/*
 *@brief ���������� �������� ����������� ��� ������ ��� ����� ��������
 *
 *@param typeFilter ��� ����������� ������
 *@param properties �������������� ��������� ���� ������
 *@return ����������� ��� ������
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
 *@brief ������� ����������� ��������
 *
 *@param srcBuffer	������ ��������
 *@param dstBuffer	������ �������
 *@param size		������ ���������� �������
 *@return
 */
void  Buffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset)
{
	//������ ��������� ������
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = render->transfert�ommandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(render->device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; //������������� ��� ����� ���������� ���� ���
	//���������� � ���� ������� �����������
	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = srcOffset; // Optional
	copyRegion.dstOffset = dstOffset; // Optional
	copyRegion.size = size;

	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	vkEndCommandBuffer(commandBuffer);

	//���������� �� ����������
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;


	vkQueueSubmit(render->transfertQueue, 1, &submitInfo, VK_NULL_HANDLE);
	//���������� ����������
	vkQueueWaitIdle(render->transfertQueue);

	vkFreeCommandBuffers(render->device, render->transfert�ommandPool, 1, &commandBuffer);
}

void Buffer::allocBuffer(VkDeviceSize size, VkBufferUsageFlags usage)
{
	bufferSize = size;
	vkBufferFlags = usage;
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	//������ ������
	if (vkCreateBuffer(render->device, &bufferInfo, nullptr, &vkBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create buffer!");
	}
}

void  Buffer::allocMemory(VkDeviceSize size, VkMemoryPropertyFlags properties)
{
	bufferMemoryFlags = properties;
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(render->device, vkBuffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);
	//�������� ������
	if (vkAllocateMemory(render->device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate buffer memory!");
	}
}

void Buffer::deallocBuffer()
{
	vkDestroyBuffer(render->device, vkBuffer, nullptr);
	buffer_complite = false;
	bufferSize = 0;
}

void Buffer::deallocMemory()
{
	vkFreeMemory(render->device, bufferMemory, nullptr);
}

void Buffer::bindMemory()
{
	vkBindBufferMemory(render->device, vkBuffer, bufferMemory, 0);
	buffer_complite = true;
}