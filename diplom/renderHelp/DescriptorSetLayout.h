#pragma once
#include "../stdafx.h"

class DescriptorSetLayout
{
public:
	DescriptorSetLayout();
	DescriptorSetLayout(VkDevice curentDevice);
	~DescriptorSetLayout();
	//��������� ��������� �����������
	DescriptorSetLayout(const DescriptorSetLayout& other) = delete;
	DescriptorSetLayout& operator= (const DescriptorSetLayout& other) = delete;
	//��������� ��������� �����������
	DescriptorSetLayout(DescriptorSetLayout&& that);
	DescriptorSetLayout& operator= (DescriptorSetLayout&& that);

	friend void swap(DescriptorSetLayout& lhs, DescriptorSetLayout& rhs) noexcept
	{
		std::swap(lhs.descriptorSetLayout, rhs.descriptorSetLayout);
		std::swap(lhs.device, rhs.device);
	}

	VkDescriptorSetLayout& getDescriptorSetLayout(); //�� ������� ������� � �������������, �� ��� ������ ����� ���������
private:
	VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;


};