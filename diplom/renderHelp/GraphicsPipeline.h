#pragma once
#include "../stdafx.h"

/*
* Класс графический конвеер
* Служит для конструирования выдачи и удаления указзателя на графический конвер вулкана
* Наиболее большая и запутанная часть - создание
*/
class GraphicsPipeline
{
public:
	GraphicsPipeline();
	GraphicsPipeline(VkDevice curentDevice, VkGraphicsPipelineCreateInfo const* pipelineInfo);
	~GraphicsPipeline();
	//Запрещаем семантику копирования 
	GraphicsPipeline(GraphicsPipeline const & other) = delete;
	GraphicsPipeline& operator= (GraphicsPipeline const& other) = delete; //Здесь мы копируем данные из lvalue в наш объект. Но мы его запретим создавать явно
	//Определяем семантику перемещения
	
	GraphicsPipeline(GraphicsPipeline&& that); //Здесь мы перемещаем данные из rvalue, которому скоро умирать, в наш объект
	GraphicsPipeline& operator=(GraphicsPipeline&& that); 	

	friend void swap(GraphicsPipeline& lhs, GraphicsPipeline& rhs) noexcept
	{
		std::swap(lhs.graphicsPipeline, rhs.graphicsPipeline);
		std::swap(lhs.device, rhs.device);
	}

	VkPipeline& GetPipeline();
private:
	VkPipeline graphicsPipeline = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;

};