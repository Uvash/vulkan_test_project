#include "stdafx.h"
#include "ShaderLoader.h"

using namespace renderHelp;

ShaderStages::ShaderStages(VkDevice newDevice) : device(newDevice)
{

}

ShaderStages::~ShaderStages()
{
}

void ShaderStages::loadShaders(int shadersType)
{
	if (!device)
	{
		throw std::runtime_error("ShaderStages request VkDevice*");
	}
	switch (shadersType)
	{
		case shaderType::STATIC_COLOR:
		{
			shaders.push_back(Shader{ device, "shaders/vert.spv", "main", VK_SHADER_STAGE_VERTEX_BIT });
			shaders.push_back(Shader{ device, "shaders/frag.spv", "main", VK_SHADER_STAGE_FRAGMENT_BIT });
			break;
		}
		case shaderType::DYNAMIC_COLOR:
		{
			shaders.push_back(Shader{ device, "shaders/colorShader.spv", "main", VK_SHADER_STAGE_VERTEX_BIT });
			shaders.push_back(Shader{ device, "shaders/frag.spv", "main", VK_SHADER_STAGE_FRAGMENT_BIT });
			break;
		}
		default:
		{
			throw std::runtime_error("ShaderStages unknown shaders type ");
		}
	}


	for (auto shader = shaders.begin(); shader != shaders.end(); shader++)
	{
		shaderStages.push_back(VkPipelineShaderStageCreateInfo{ shader->getShaderModule() });
	}
}