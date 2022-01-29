#pragma once

#include "stdafx.h"
class Render;

class Buffer
{
protected:
	Render* render;
	VkBuffer vkBuffer = VK_NULL_HANDLE;
	VkBufferUsageFlags vkBufferFlags = 0x0;
	VkDeviceSize bufferSize = 0;
	VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
	VkMemoryPropertyFlags  bufferMemoryFlags = 0x0;

	void allocBuffer(VkDeviceSize size, VkBufferUsageFlags usage);
	void allocMemory(VkMemoryPropertyFlags properties);
	void deallocBuffer();
	void deallocMemory();
	void bindMemory();

public:
	Buffer() = delete;
	virtual ~Buffer();

	Buffer(Render& new_render);

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
	void clearBuffer();
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDeviceSize srcOffset = 0, VkDeviceSize dstOffset = 0);
	void copyToBuffer(void* srcPtr, VkDeviceSize num);
	void copyFromBuffer(void* srcPtr, VkDeviceSize num, VkDeviceSize offset);

	Buffer(Buffer& other) = delete;
	Buffer& operator=(Buffer& other) = delete;

	Buffer(Buffer && that);
	Buffer& operator=(Buffer&& that);

	inline VkBuffer& getVkBufferHandle()& { return vkBuffer; }
	inline VkDeviceMemory& getVkMemoryHandle()& { return bufferMemory; }
	inline VkDeviceSize getBufferSize() { return bufferSize; }

	friend void swap(Buffer& lhs, Buffer& rhs) noexcept
	{
		std::swap(lhs.render, rhs.render);
		std::swap(lhs.vkBuffer, rhs.vkBuffer);
		std::swap(lhs.vkBufferFlags, rhs.vkBufferFlags);
		std::swap(lhs.bufferSize, rhs.bufferSize);
		std::swap(lhs.bufferMemory, rhs.bufferMemory);
		std::swap(lhs.bufferMemoryFlags, rhs.bufferMemoryFlags);
	}
};