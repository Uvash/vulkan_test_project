#pragma once
#include "../stdafx.h"
#include "../Buffer.h"
#include "../Render.h"
#include "../renderHelp/GraphicsPipeline.h"

class Primitive
{
public:
	Primitive() = delete;
	Primitive(Render& newRender, size_t newCommandBuffersCount, size_t newTargetPipeline, std::string primitiveName = "default_name");
	virtual ~Primitive() = 0;

	VkCommandBuffer& getCommandBuffer(size_t bufferNumber);
	//virtual void addComandsToCommandBuffer(VkCommandBuffer& commandBuffer, VkDescriptorSet& descriptionSet, VkPipelineLayout &pipelineLayout);
	void createCommandsBuffers();
	void clearCommandsBuffers();

	virtual void fillCommandsBuffers();
	virtual void updateCommandsBuffers(size_t bufferNumber);

	inline const std::string& getName() { return name; }
	inline const size_t& getTargetPipeline() { return targetPipeline; }

protected:
	void allocCommandBuffers(size_t newCommandBuffersCount );
	Render& render;

	size_t targetPipeline = 0; //Номер конвеера, для которого предназначен примитив
	std::vector<VkCommandBuffer> commandBuffers;
	std::string name;
	std::size_t commandBuffersCount;
protected:
	inline VkQueue& getGraphicsQueue(){ return render.graphicsQueue;  }
	inline VkDevice& getDevice() { return render.device; }
	inline VkRenderPass & getRenderPass() { return render.renderPass; }
	inline VkFramebuffer & getFramebuffer(size_t framebufferNumber) { return render.swapChainFramebuffers[framebufferNumber]; }
	inline VkExtent2D & getExtent2D() { return render.swapChainExtent; }
	inline VkPipeline& getGraphicsPipeline(size_t PipelineNumber) { return render.graphicsPipelines[PipelineNumber].GetPipeline(); }
	inline VkDescriptorSet& getDescriptorSet(size_t DescriptorSetNumber) { return render.descriptorSets[DescriptorSetNumber]; }
	inline VkPipelineLayout& getPipelineLayout() { return render.pipelineLayout; }
};