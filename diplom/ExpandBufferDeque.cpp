
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

void ExpandBufferDeque::addBuffer(Buffer& newBuffer)
{
	if (newBuffer.getBufferSize() + hotBuffer.getUsingMemorySize() > hotBuffer.getBufferSize())
	{
		sendHotBufferToColdBuffers();
	}

	hotBuffer.addBuffer(newBuffer);
}

void ExpandBufferDeque::createBuffer(VkDeviceSize size, size_t count)
{
	elementSize = size;
	elementCount = count;
	hotBuffer.createBuffer(elementCount * elementSize, hotBufferFlags, hotMemRequirements);
	tempBuffer.createBuffer(elementSize, hotBufferFlags, hotMemRequirements);
}

void ExpandBufferDeque::sendHotBufferToColdBuffers()
{
	std::cout << "ExpandBufferDeque start addBuffer cold storage size:" << coldBuffers.size() << " " << hotBuffer.getBufferSize() << std::endl;
	Buffer* localBuffer = new Buffer(*render);

	localBuffer->createBuffer(hotBuffer.getUsingMemorySize(), coldBufferFlags, coldMemRequirements);
	localBuffer->copyBuffer(hotBuffer.getVkBufferHandle(), localBuffer->getVkBufferHandle(), hotBuffer.getUsingMemorySize(), 0, 0);

	tempBuffer.createBuffer(elementSize, hotBufferFlags, hotMemRequirements);
	tempBuffer.copyBuffer(hotBuffer.getVkBufferHandle(), tempBuffer.getVkBufferHandle(), elementSize, hotBuffer.getUsingMemorySize() - elementSize, 0);

	coldBuffers.push_back(localBuffer);
	hotBuffer.resetComplexBuffer();
	hotBuffer.addBuffer(tempBuffer);
}

ExpandBufferDeque::ExpandBufferDeque(Render& newRender) : hotBuffer{ newRender }, tempBuffer{ newRender }
{
	render = &newRender;

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