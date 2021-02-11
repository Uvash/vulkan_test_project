#pragma once
#include "stdafx.h"

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily; //Номер очереди для отрисовки
	std::optional<uint32_t> presentFamily;	//Номер очереди для показа
	std::optional<uint32_t> transferFamily;	//Номер очереди для управления памятью

	//Все ли очереди инициализированы	
	bool isComplete()
	{
		return	graphicsFamily.has_value() &&
			presentFamily.has_value() &&
			transferFamily.has_value();
	}
};

//Струкутра описывающая свойства цепочки обмена
struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};