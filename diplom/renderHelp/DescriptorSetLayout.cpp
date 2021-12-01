#include "stdafx.h"
#include "DescriptorSetLayout.h"

DescriptorSetLayout::DescriptorSetLayout()
{

}

DescriptorSetLayout::DescriptorSetLayout(VkDevice curentDevice)
{
	if (curentDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("DescriptorSetLayout: Device handler is null");
	}

	device = curentDevice;

	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; //Указываем что будем использовать только в вершинном шейдере
	uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &uboLayoutBinding;

	if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("DescriptorSetLayout: failed to create descriptor set layout!");
	}
}

DescriptorSetLayout::~DescriptorSetLayout()
{
	if (device != VK_NULL_HANDLE && descriptorSetLayout != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
	}
}

DescriptorSetLayout::DescriptorSetLayout(DescriptorSetLayout&& that) : device (VK_NULL_HANDLE), descriptorSetLayout(VK_NULL_HANDLE)
{
	swap(*this, that);
}

DescriptorSetLayout& DescriptorSetLayout::operator= (DescriptorSetLayout&& that)
{
	swap(*this, that);
	return *this;
}

VkDescriptorSetLayout& DescriptorSetLayout::getDescriptorSetLayout()
{
	return descriptorSetLayout;
}