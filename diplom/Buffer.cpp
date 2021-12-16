#include "stdafx.h"

#include "Buffer.h"
#include "Render.h"


Buffer::~Buffer()
{
	clearBuffer();
}

Buffer::Buffer(Render& new_render) : render(&new_render)
{

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
	if (vkBuffer != VK_NULL_HANDLE || bufferMemory != VK_NULL_HANDLE)
	{
		clearBuffer();
	}
	allocBuffer(size, usage);
	allocMemory(properties);
	bindMemory();
}

void Buffer::clearBuffer()
{
	deallocBuffer();
	deallocMemory();
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
	throw std::runtime_error("Buffer : failed to find suitable memory type!");
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
	bufferInfo.size = bufferSize;
	bufferInfo.usage = vkBufferFlags;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	//������ ������
	if (vkCreateBuffer(render->device, &bufferInfo, nullptr, &vkBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("Buffer : failed to create buffer!");
	}
}

void  Buffer::allocMemory(VkMemoryPropertyFlags properties)
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
		throw std::runtime_error("Buffer : failed to allocate buffer memory!");
	}
}

void Buffer::deallocBuffer()
{
	if (vkBuffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(render->device, vkBuffer, nullptr);
		vkBuffer = VK_NULL_HANDLE;
	}
	bufferSize = 0;
}

void Buffer::deallocMemory()
{
	if (bufferMemory != VK_NULL_HANDLE)
	{
		vkFreeMemory(render->device, bufferMemory, nullptr);
		bufferMemory = VK_NULL_HANDLE;
	}
}

void Buffer::bindMemory()
{
	if (vkBuffer == VK_NULL_HANDLE || bufferMemory == VK_NULL_HANDLE)
	{
		throw std::runtime_error("Buffer : try bind buffer and memory with null handle !");
	}
	vkBindBufferMemory(render->device, vkBuffer, bufferMemory, 0);
}

Buffer::Buffer(Buffer&& that)
{
	swap(*this, that);
}

Buffer& Buffer::operator=(Buffer&& that)
{
	swap(*this, that);
	return *this;
}