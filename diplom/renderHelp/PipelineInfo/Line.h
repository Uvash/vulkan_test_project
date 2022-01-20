#pragma once
#include "../../stdafx.h"
#include "../PipelineInfo.h"
class Render;

namespace Pipeline
{
	class Line : public PipelineInfo
	{
	public:
		Line(Render& newRender);
		~Line();
		void  assemblePipelineInfo() override;
	protected:
		void createInputAssembly() override;
		void setAttributeDescription() override;
	};
}