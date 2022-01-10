#pragma once
#include "../stdafx.h"
#include "../Buffer.h"

class Render;
class Primitive
{
public:
	Primitive() = delete;
	Primitive(Render& newRender);
	virtual ~Primitive() = default;

	std::shared_ptr<Buffer> pBuffer;
	
	virtual void addComandsToCommandBuffer(VkCommandBuffer& commandBuffer, VkDescriptorSet& descriptionSet, VkPipelineLayout &pipelineLayout);
};