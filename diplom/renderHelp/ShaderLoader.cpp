#include "stdafx.h"
#include "ShaderLoader.h"

using namespace renderHelp;

ShaderStages::ShaderStages(VkDevice newDevice) : device(newDevice)
{
	if (!device)
	{
		throw std::runtime_error("ShaderStages request VkDevice*");
	}

	shaders.push_back(Shader{ device, "shaders/vert.spv", "main", VK_SHADER_STAGE_VERTEX_BIT });
	shaders.push_back(Shader{ device, "shaders/frag.spv", "main", VK_SHADER_STAGE_FRAGMENT_BIT });

	for (auto shader = shaders.begin(); shader != shaders.end(); shader++)
	{
		shaderStages.push_back(VkPipelineShaderStageCreateInfo{shader->getShaderModule()});
	}
}

ShaderStages::~ShaderStages()
{
}
