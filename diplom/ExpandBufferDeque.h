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

	void sendHotBufferToColdBuffers();	//�������� ������ �� �������� ������ � ��������
	std::vector<Buffer*> coldBuffers; //�������� ������ ��������� ������ GPU
	ComplexBuffer hotBuffer;	//������� �����, ��������� ��� CPU � GPU

	VkBufferUsageFlags hotBufferFlags;
	VkMemoryPropertyFlags hotMemRequirements;
	VkBufferUsageFlags coldBufferFlags;
	VkMemoryPropertyFlags coldMemRequirements;

	VkDeviceSize elementCountPerBuffer;
	VkDeviceSize elementSize;
};