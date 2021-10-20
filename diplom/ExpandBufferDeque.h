#pragma once

#include "stdafx.h"
#include "ComplexBuffer.h"

class Buffer;
class ComplexBuffer;
class Render;


class ExpandBufferDeque
{
friend Render;
public:
	void setHotBufferSpecificParameters(VkBufferUsageFlags newBufferFlags, VkMemoryPropertyFlags newMemRequirements);
	void setColdBufferSpecificParameters(VkBufferUsageFlags newBufferFlags, VkMemoryPropertyFlags newMemRequirements);

	void addBuffer(Buffer* newBuffer);
	void initBuffer(Render* newRender);
	void createBuffer(VkDeviceSize size);

	ExpandBufferDeque();
	~ExpandBufferDeque();

private:
	Render* render;

	void sendHotBufferToColdBuffers();	//Копирует данные из горячего буфера в холодный
	std::vector<Buffer*> coldBuffers; //Холодные буфера доступные только GPU
	ComplexBuffer hotBuffer;	//Горячий буфер, доступные для CPU и GPU

	VkBufferUsageFlags hotBufferFlags;
	VkMemoryPropertyFlags hotMemRequirements;
	VkBufferUsageFlags coldBufferFlags;
	VkMemoryPropertyFlags coldMemRequirements;

	VkDeviceSize elementCountPerBuffer;
	VkDeviceSize elementSize;
};