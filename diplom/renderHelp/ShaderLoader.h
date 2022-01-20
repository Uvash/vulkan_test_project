#pragma once
#include "../stdafx.h"
#include "Shader.h"

namespace renderHelp
{
	enum shaderType
	{
		STATIC_COLOR, //������ ����� ��� �������� �������� ������� vec3 � �������� ���� ����������
		DYNAMIC_COLOR //������ ����� ������� �� ���� vec3 ��� ��������� � �����
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