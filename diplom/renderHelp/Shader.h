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
		//ƒл€ вектора кода, имени, и структуры возможно стоит использовать семантику перемещени€ и чЄт мне кажетьс€ что возможны исключени€ в при свапе массивов из-за выделени€ пам€ти
		std::swap(lhs.device, rhs.device);
		std::swap(lhs.rawShaderModule, rhs.rawShaderModule);
		std::swap(lhs.warpedShaderModule, rhs.warpedShaderModule);
		lhs.code = std::move(rhs.code);
		lhs.entryPointName = std::move(rhs.entryPointName);
		/*
			»з-за того что entryPointName может €вл€тьс€ SSO строкой (пример "main") и полностью оставатьс€ на стеке
			—емантика перемещени€ фактически не работает и будет измен€ть указатель на начало строки согласованный с warpedShaderModule.pName
			“ак что нам придЄтс€ обновл€ть указатель на название функции в warpedShaderModule руками
		*/
		lhs.warpedShaderModule.pName = lhs.entryPointName.c_str();
	}
};