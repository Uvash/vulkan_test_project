#include "stdafx.h"
#include "Line.h"

using namespace Pipeline;

Line::Line(Render& newRender) : PipelineInfo(newRender)
{

}

Line::~Line()
{

}

void Line::createInputAssembly()
{
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;
}
