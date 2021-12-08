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
	protected:
		void createInputAssembly() override;
	};
}
