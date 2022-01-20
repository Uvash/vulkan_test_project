#include "stdafx.h"
#include "StripLine.h"

using namespace Pipeline;

StripLine::StripLine(Render & newRender) : PipelineInfo(newRender)
{

}

StripLine::~StripLine()
{

}

void  StripLine::assemblePipelineInfo()
{
	setAttributeDescription();
	createShaders(renderHelp::shaderType::STATIC_COLOR);
	createVertexInputInfo();
	createInputAssembly();
	createPipelineInfo();
}

void StripLine::createInputAssembly()
{
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
	inputAssembly.primitiveRestartEnable = VK_FALSE;
}
