
#include "stdafx.h"

#include "Render.h"
#include "Buffer.h"
#include "ComplexBuffer.h"
#include "renderHelp/HelpStructures.h"
#include "ExpandBufferDeque.h"

void ExpandBufferDeque::setHotBufferSpecificParameters(VkBufferUsageFlags newBufferFlags, VkMemoryPropertyFlags newMemRequirements)
{
	hotBufferFlags = newBufferFlags;
	hotMemRequirements = newMemRequirements;
}

void ExpandBufferDeque::setColdBufferSpecificParameters(VkBufferUsageFlags newBufferFlags, VkMemoryPropertyFlags newMemRequirements)
{
	coldBufferFlags = newBufferFlags;
	coldMemRequirements = newMemRequirements;
}

void ExpandBufferDeque::addBuffer(Buffer* newBuffer)
{
	if (newBuffer->getBufferSize() + hotBuffer.getUsingMemorySize() > hotBuffer.getBufferSize())
	{
		sendHotBufferToColdBuffers();
	}

	hotBuffer.addBuffer(newBuffer);
}

void ExpandBufferDeque::initBuffer(Render* newRender)
{
	render = newRender;
	hotBuffer.initBuffer(render);
}

void ExpandBufferDeque::createBuffer(VkDeviceSize size)
{
	hotBuffer.createBuffer(size, hotBufferFlags, hotMemRequirements);
}

void ExpandBufferDeque::sendHotBufferToColdBuffers()
{
	std::cout << "ExpandBufferDeque start addBuffer cold storage size:" << coldBuffers.size() << " " << hotBuffer.getBufferSize() << std::endl;
	Buffer* localBuffer = new Buffer();
	localBuffer->initBuffer(render);
	localBuffer->createBuffer(hotBuffer.getUsingMemorySize(), hotBufferFlags, hotMemRequirements);
	localBuffer->copyBuffer(hotBuffer.getVkBufferHandle(), localBuffer->getVkBufferHandle(), hotBuffer.getUsingMemorySize(), 0, 0);
	coldBuffers.push_back(localBuffer);

	hotBuffer.resetComplexBuffer();
}

ExpandBufferDeque::ExpandBufferDeque()
{
	
}

ExpandBufferDeque::~ExpandBufferDeque()
{
	for (auto buffer : coldBuffers)
	{
		if (!buffer)
		{
			continue;
		}

		delete buffer;
	}
}