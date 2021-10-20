#pragma once

#include "stdafx.h"
#include "Buffer.h"

class ComplexBuffer : public Buffer
{
public:
	virtual ~ComplexBuffer();
	void resetComplexBuffer();
	void addBuffer(Buffer* newBuffer);
	void addComplexBuffer(ComplexBuffer* newBuffer);
	inline VkDeviceSize getUsingMemorySize() { return usingMemorySize; }
protected:
	VkDeviceSize usingMemorySize; // Количество памяти, содержащее полезную информацию
	virtual void allocBuffer(VkDeviceSize size, VkBufferUsageFlags usage);
};