#pragma once
#include "../stdafx.h"
#include "../ExpandBufferDeque.h"
#include "Primitive.h"

class AbstractItertionMetod;

class CurvedPrimitive : public Primitive
{
public:
	CurvedPrimitive() = delete;
	CurvedPrimitive(Render& newRender, size_t newCommandBuffersCount, size_t newTargetPipeline, std::string primitiveName = "default_name");
	~CurvedPrimitive() override;

	void fillCommandsBuffers() override;
	void updateCommandsBuffers(size_t bufferNumber) override;
	void updateInformation();
	void connectToItertionMetod(AbstractItertionMetod* metod);
protected:
	std::shared_ptr<ExpandBufferDeque> pBufferDeque;
	std::shared_ptr<Buffer> swapBuffer;
	AbstractItertionMetod* iterationMetod;
};
