#pragma once

#include "stdafx.h"
class Render;

class Buffer
{
private:
	Render* render;
	VkBuffer vkBuffer = VK_NULL_HANDLE;
	VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
	bool init_complite = false;
	bool buffer_complite = false;
	VkDeviceSize bufferSize;

public:
	Buffer();
	~Buffer();

	void initBuffer(Render* new_render);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void  copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	inline VkBuffer getVkBufferHandle() { return vkBuffer; }
	inline VkDeviceMemory  getVkMemoryHandle() { return bufferMemory; }

};