#include "stdafx.h"
#include "StaticPrimitive.h"
#include "../Render.h"
#include "../renderHelp/HelpStructures.h"


StaticPrimitive::StaticPrimitive(Render& newRender, size_t newCommandBuffersCount, size_t newTargetPipeline, std::string primitiveName) : Primitive(newRender, newCommandBuffersCount, newTargetPipeline, primitiveName), pBuffer{ std::make_shared<Buffer>(newRender) }
{

}

StaticPrimitive::~StaticPrimitive()
{

}

void StaticPrimitive::loadFromFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios_base::in);
	if (!file.is_open())
	{
		throw std::runtime_error("failed to open file!");
	}


	VertexColored vertex;
	int i = 0;
	std::vector<VertexColored> rawIndexes{};

	float x, y, z, cx, cy, cz;
	while (file >> x >> y >> z >> cx >> cy >> cz)
	{
		VertexColored vertex;
		//file >> vertex.pos.x >> vertex.pos.y >> vertex.pos.z >> vertex.color.x >> vertex.color.y >> vertex.color.z;		
		vertex = VertexColored{ {x, y, z}, {cx, cy, cz} };
		rawIndexes.push_back(VertexColored{ {x, y, z}, {cx, cy, cz} });
	}
	//Закрываем файл
	file.close();

	pBuffer->createBuffer(rawIndexes.size() * sizeof(rawIndexes[0]), VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	pBuffer->copyToBuffer(rawIndexes.data(), rawIndexes.size() * sizeof(rawIndexes[0]));

	fillCommandsBuffers();
}

void StaticPrimitive::fillCommandsBuffers()
{
	for (size_t i = 0; i < commandBuffers.size(); i++)
	{
		VkCommandBufferInheritanceInfo BufferInheritanceInfo{};
		BufferInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		BufferInheritanceInfo.renderPass = getRenderPass();
		BufferInheritanceInfo.framebuffer = getFramebuffer(i);
		BufferInheritanceInfo.occlusionQueryEnable = VK_FALSE; // если что-то заблокирует основной буфер, то будем ждать его разблокировки

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT; // Optional
		beginInfo.pInheritanceInfo = &BufferInheritanceInfo; // Optional

		if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = getRenderPass();
		renderPassInfo.framebuffer = getFramebuffer(i);
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = getExtent2D();
		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 0.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		//vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		//Отрисовываем координатную сетку
		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, getGraphicsPipeline(targetPipeline));
		VkDeviceSize offset = 0;
		vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, &pBuffer->getVkBufferHandle(), &offset);

		vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, getPipelineLayout(), 0, 1, &getDescriptorSet(i), 0, nullptr);
		vkCmdDraw(commandBuffers[i], static_cast<uint32_t>(pBuffer->getBufferSize() / sizeof(VertexColored)), 1, 0, 0);

		//закрываем буфер на запись
		//vkCmdEndRenderPass(commandBuffers[i]);

		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("StaticPrimitive: failed to record command buffer!");
		}
	}
}
