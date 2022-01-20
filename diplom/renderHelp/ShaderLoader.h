#pragma once
#include "../stdafx.h"
#include "Shader.h"

namespace renderHelp
{
	enum shaderType
	{
		STATIC_COLOR, //Каждая точка для конвеера является простым vec3 и содержит свои координаты
		DYNAMIC_COLOR //Каждая точка состоит из двух vec3 для координат и цвета
	};

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