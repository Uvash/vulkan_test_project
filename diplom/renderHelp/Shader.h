#pragma once
#include "../stdafx.h"

class Shader
{
public:
	Shader();
	~Shader();
	Shader(VkDevice newDevice, std::string filename, std::string newEntryPointName, VkShaderStageFlagBits stage);
	
	Shader(const Shader& other) = delete;
	Shader& operator= (const Shader& other) = delete;

	Shader(Shader&& that) noexcept;
	Shader& operator= (Shader&& that) noexcept;

	VkPipelineShaderStageCreateInfo& getShaderModule() &;
public:
	VkDevice device = VK_NULL_HANDLE;

	VkShaderModule rawShaderModule = VK_NULL_HANDLE;
	VkPipelineShaderStageCreateInfo warpedShaderModule{};

	std::vector<char> code;
	std::string entryPointName;

	std::vector<char> readFile(const std::string & filename);
public:
	friend void swap(Shader& lhs, Shader& rhs) noexcept
	{
		//��� ������� ����, �����, � ��������� �������� ����� ������������ ��������� ����������� � ��� ��� �������� ��� �������� ���������� � ��� ����� �������� ��-�� ��������� ������
		std::swap(lhs.device, rhs.device);
		std::swap(lhs.rawShaderModule, rhs.rawShaderModule);
		std::swap(lhs.warpedShaderModule, rhs.warpedShaderModule);
		lhs.code = std::move(rhs.code);
		lhs.entryPointName = std::move(rhs.entryPointName);
		/*
			��-�� ���� ��� entryPointName ����� �������� SSO ������� (������ "main") � ��������� ���������� �� �����
			��������� ����������� ���������� �� �������� � ����� �������� ��������� �� ������ ������ ������������� � warpedShaderModule.pName
			��� ��� ��� ������� ��������� ��������� �� �������� ������� � warpedShaderModule ������
		*/
		lhs.warpedShaderModule.pName = lhs.entryPointName.c_str();
	}
};