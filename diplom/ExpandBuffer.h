#pragma once

#include "stdafx.h"
#include "Buffer.h"

class ExpandBuffer : public Buffer
{
private:
	VkDeviceSize realMemorySize = 0;
	VkFence fence;
	virtual void allocMemory(VkDeviceSize size, VkMemoryPropertyFlags properties);
public:
	virtual ~ExpandBuffer();
	void expand(Buffer * newBuffer);

};