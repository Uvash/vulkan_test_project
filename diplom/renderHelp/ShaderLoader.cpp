#include "stdafx.h"
#include "ShaderLoader.h"

using namespace renderHelp;

void ShaderStages::init(VkDevice* newDevice)
{
	if (!newDevice)
	{
		throw std::runtime_error("ShaderStages request VkDevice*");
	}

	device = newDevice;
}

void ShaderStages::createStagingBuffer()
{
	//����� �������� ������������ �������� �� ����� ������� ����� � ����� �������� � ������ ��� ����
	//�������� ��������� �������
	auto vertShaderCode = readFile("shaders/vert.spv");
	auto fragShaderCode = readFile("shaders/frag.spv");
	//�������������� ������� ��� ������
	vkShaderModules.push_back(createShaderModule(vertShaderCode));
	vkShaderModules.push_back(createShaderModule(fragShaderCode));

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	//��������� ����� ��� ������� � �� ����� ����� ������������ �������� ����� �����������.
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	//��������� �������� ������ � ����� �����
	vertShaderStageInfo.module = vkShaderModules[0];
	vertShaderStageInfo.pName = "main";

	shaderStages.push_back(vertShaderStageInfo);

	//��������� ��������� ��� ������������ �������
	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = vkShaderModules[1];
	fragShaderStageInfo.pName = "main";

	shaderStages.push_back(fragShaderStageInfo);
}

ShaderStages::~ShaderStages()
{
	for (auto shaderModule : vkShaderModules)
	{
		vkDestroyShaderModule(*device, shaderModule, nullptr);
	}
}

std::vector<char> ShaderStages::readFile(const std::string& filename)
{
	//������ � ����� � ��� �������� ���� (��� �� �� ���� ������ ���������)
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	if (!file.is_open())
	{
		throw std::runtime_error("failed to open file!");
	}
	//��������� ������ � �����, �� ����� ������ �����, � �������� ��� ���� ������
	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);
	//������������ � ������ � �������� ������
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	//��������� ����
	file.close();
	return buffer;
}

VkShaderModule ShaderStages::createShaderModule(const std::vector<char>& code)
{
	if (!device)
	{
		throw std::runtime_error("ShaderStages request VkDevice*");
	}

	//������ ��������� ������
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	//�������� ������� ������
	VkShaderModule shaderModule;
	if (vkCreateShaderModule(*device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}