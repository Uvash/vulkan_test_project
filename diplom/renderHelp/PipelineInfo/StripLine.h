#pragma once
#include "../../stdafx.h"
#include "../PipelineInfo.h"
class Render;
namespace Pipeline
{
	class StripLine : public PipelineInfo
	{
	public:
		StripLine(Render& newRender);
		~StripLine();
		void  assemblePipelineInfo() override;
	protected:
		void createInputAssembly() override;
	};
}
