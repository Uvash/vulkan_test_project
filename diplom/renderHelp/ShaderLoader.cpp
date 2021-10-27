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
	//После создания графического конвеера мы можем удалять файлы с кодом шейдеров и обёртки над ними
	//Пытаемся загрузить шейдеры
	auto vertShaderCode = readFile("shaders/vert.spv");
	auto fragShaderCode = readFile("shaders/frag.spv");
	//Подготавливаем шейдеры для работы
	vkShaderModules.push_back(createShaderModule(vertShaderCode));
	vkShaderModules.push_back(createShaderModule(fragShaderCode));

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	//Указываем какой тип шейдера и на каком этапе графического конвеера будет использован.
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	//Указываем название модуля и точку входа
	vertShaderStageInfo.module = vkShaderModules[0];
	vertShaderStageInfo.pName = "main";

	shaderStages.push_back(vertShaderStageInfo);

	//Повторяем процедуру для фрагментного шейдера
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

VkShaderModule ShaderStages::createShaderModule(const std::vector<char>& code)
{
	if (!device)
	{
		throw std::runtime_error("ShaderStages request VkDevice*");
	}

	//Создаём структуру модуля
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	//Пытаемся создать обёртку
	VkShaderModule shaderModule;
	if (vkCreateShaderModule(*device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}