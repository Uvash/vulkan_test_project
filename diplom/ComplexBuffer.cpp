#include "stdafx.h"
#include "ComplexBuffer.h"
ComplexBuffer::~ComplexBuffer()
{
}

void ComplexBuffer::resetComplexBuffer()
{
	usingMemorySize = 0;
}

void ComplexBuffer::addBuffer(Buffer* newBuffer)
{
	if (usingMemorySize + newBuffer->getBufferSize() > bufferSize)
	{
		throw std::runtime_error("ComplexBuffer: shrink memory");
	}

	copyBuffer(newBuffer->getVkBufferHandle(), vkBuffer, newBuffer->getBufferSize(), 0, usingMemorySize);

	usingMemorySize += newBuffer->getBufferSize();
}

void ComplexBuffer::addComplexBuffer(ComplexBuffer* newBuffer)
{
	if (usingMemorySize + newBuffer->getUsingMemorySize() > bufferSize)
	{
		throw std::runtime_error("ComplexBuffer: shrink memory");
	}

	copyBuffer(newBuffer->getVkBufferHandle(), vkBuffer, newBuffer->getUsingMemorySize(), 0, usingMemorySize);

	usingMemorySize += newBuffer->getUsingMemorySize();
}

void ComplexBuffer::allocBuffer(VkDeviceSize size, VkBufferUsageFlags usage)
{
	usingMemorySize = 0;
	Buffer::allocBuffer(size, usage);
}
