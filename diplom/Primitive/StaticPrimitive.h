#pragma once

#include "../stdafx.h"
#include "../Buffer.h"
#include "Primitive.h"

class StaticPrimitive : public Primitive
{
public:
	StaticPrimitive() = delete;
	StaticPrimitive(Render& newRender, size_t newCommandBuffersCount, size_t newTargetPipeline, std::string primitiveName);
	~StaticPrimitive() override;
	void fillCommandsBuffers() override;

	void loadFromFile(const std::string& filename);
protected:
	std::shared_ptr<Buffer> pBuffer;
};