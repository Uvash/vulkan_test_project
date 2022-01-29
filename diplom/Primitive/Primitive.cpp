#include "stdafx.h"
#include "Primitive.h"
#include "../renderHelp/HelpStructures.h"
#include "../Buffer.h"

Primitive::Primitive(Render& newRender) : pBuffer{ std::make_shared<Buffer>(newRender) }
{

}

void Primitive::addComandsToCommandBuffer(VkCommandBuffer& commandBuffer, VkDescriptorSet& descriptionSet, VkPipelineLayout& pipelineLayout)
{
	VkDeviceSize offset =  0;
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &pBuffer->getVkBufferHandle(), &offset);

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptionSet, 0, nullptr);
	vkCmdDraw(commandBuffer, static_cast<uint32_t>(pBuffer->getBufferSize() / sizeof(VertexColored)), 1, 0, 0);
}

void Primitive::loadFromFile(const std::string& filename)
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
	
}