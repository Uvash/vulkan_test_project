#pragma once
#include "../stdafx.h"
#include "Shader.h"

namespace renderHelp
{
	class ShaderStages
	{
	public:
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

		ShaderStages(VkDevice newDevice);
		~ShaderStages();
	private:
		VkDevice device;
		std::vector<Shader> shaders;	
	};
}