#pragma once
#include "../stdafx.h"

namespace renderHelp
{
	class ShaderStages
	{
	public:
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

		void init(VkDevice* newDevice);
		void createStagingBuffer();
		~ShaderStages();
	private:
		VkDevice *device;
		std::vector<VkShaderModule> vkShaderModules;
		static std::vector<char> readFile(const std::string& filename);
		VkShaderModule createShaderModule(const std::vector<char>& code);		
	};
}