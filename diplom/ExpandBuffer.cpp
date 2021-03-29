#pragma once

#include "stdafx.h"
#include "ExpandBuffer.h"


ExpandBuffer::~ExpandBuffer()
{
}

void ExpandBuffer::allocMemory(VkDeviceSize size, VkMemoryPropertyFlags properties)
{

	realMemorySize = size * 16;
	Buffer::allocMemory(realMemorySize, properties);
}


void ExpandBuffer::expand(Buffer * newBuffer)
{
	ExpandBuffer* t = this;
	VkDeviceSize oldBufferSize = bufferSize;

	if (bufferSize + newBuffer->getBufferSize() > realMemorySize)
	{
		throw std::runtime_error("ExpandBuffer: shrink memory");
	}

	if (init_complite && buffer_complite)
	{
		deallocBuffer();
	}

	VkDeviceSize newBufferSize = oldBufferSize + newBuffer->getBufferSize();
	allocBuffer(newBufferSize, vkBufferFlags);
	bindMemory();	
	copyBuffer(newBuffer->getVkBufferHandle(), vkBuffer, newBuffer->getBufferSize(), 0, oldBufferSize);
	bufferSize = newBufferSize; //Сдвигаем буффер на количество записанных байт
}

