#pragma once

#include "stdafx.h"
class Render;

class Buffer
{
protected:
	Render* render;
	VkBuffer vkBuffer = VK_NULL_HANDLE;
	VkBufferUsageFlags vkBufferFlags = 0;
	VkDeviceSize bufferSize = 0;
	VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
	VkMemoryPropertyFlags  bufferMemoryFlags = 0;
	bool init_complite = false;
	bool buffer_complite = false;


	virtual void allocBuffer(VkDeviceSize size, VkBufferUsageFlags usage);
	virtual void allocMemory(VkMemoryPropertyFlags properties);
	void deallocBuffer();
	void deallocMemory();
	void bindMemory();

public:
	Buffer();
	virtual ~Buffer();

	virtual void initBuffer(Render* new_render);
	virtual void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDeviceSize srcOffset = 0, VkDeviceSize dstOffset = 0);


	inline VkBuffer getVkBufferHandle() { return vkBuffer; }
	inline VkDeviceMemory  getVkMemoryHandle() { return bufferMemory; }
	inline VkDeviceSize getBufferSize() { return bufferSize; }
};