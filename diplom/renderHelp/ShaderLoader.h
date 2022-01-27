#pragma once
#include "../stdafx.h"
#include "Shader.h"
#include "HelpStructures.h"

namespace renderHelp
{
	class ShaderStages
	{
	public:
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

		ShaderStages(VkDevice newDevice);
		~ShaderStages();

		virtual void loadShaders(int shadersType);
	protected:
		VkDevice device;
		std::vector<Shader> shaders;	
	};
}