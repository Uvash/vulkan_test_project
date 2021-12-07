#include "stdafx.h"
#include "Shader.h"

Shader::Shader()
{

}

Shader::~Shader()
{
	if(rawShaderModule != VK_NULL_HANDLE)
	{
		vkDestroyShaderModule(device, rawShaderModule, nullptr);
	}
}
Shader::Shader(VkDevice newDevice, std::string filename, std::string newEntryPointName, VkShaderStageFlagBits stage) : device(newDevice), entryPointName(newEntryPointName)
{
	code = readFile(filename);

	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	if (vkCreateShaderModule(device, &createInfo, nullptr, &rawShaderModule) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create shader module!");
	}

	//Указываем какой тип шейдера и на каком этапе графического конвеера будет использован.
	warpedShaderModule.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	warpedShaderModule.stage = stage;
	//Указываем название модуля и точку входа
	warpedShaderModule.module = rawShaderModule;
	warpedShaderModule.pName = entryPointName.c_str();
}

Shader::Shader(Shader&& that) noexcept
{
	swap(*this, that);
}

Shader& Shader::operator= (Shader&& that) noexcept
{
	swap(*this, that);
	return *this;
}

VkPipelineShaderStageCreateInfo& Shader::getShaderModule() &
{
	return warpedShaderModule;
}


std::vector<char> Shader::readFile(const std::string & filename)
{
	//Читаем с конца и как бинарный файл (что бы не было ошибок кодировки)
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	if (!file.is_open())
	{
		throw std::runtime_error("failed to open file!");
	}
	//Поскольку читаем с конца, мы узнаём размер файла, и выделяем под него память
	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);
	//Возвращаемся в начало и начинаем чтение
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	//Закрываем файл
	file.close();
	return buffer;
}