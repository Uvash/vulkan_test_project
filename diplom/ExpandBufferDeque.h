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

	void addBuffer(Buffer& newBuffer);
	void createBuffer(VkDeviceSize size, size_t count);

	ExpandBufferDeque() = delete;
	ExpandBufferDeque(Render& newRender);
	~ExpandBufferDeque();

private:
	Render* render;

	void sendHotBufferToColdBuffers();	//�������� ������ �� �������� ������ � ��������
	std::vector<Buffer*> coldBuffers; //�������� ������ ��������� ������ GPU
	ComplexBuffer hotBuffer;	//������� �����, ��������� ��� CPU � GPU
	Buffer tempBuffer;			//��������� �����, ��� ���������� "�������"

	VkBufferUsageFlags hotBufferFlags;
	VkMemoryPropertyFlags hotMemRequirements;
	VkBufferUsageFlags coldBufferFlags;
	VkMemoryPropertyFlags coldMemRequirements;

	VkDeviceSize elementCount;
	VkDeviceSize elementSize;
};