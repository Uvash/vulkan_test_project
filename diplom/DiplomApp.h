#pragma once
#include "stdafx.h"
#include "WindowManager.h"
#include "Render.h"

class DiplomApp
{

private:
	WindowManager whindow;
	Render render;

	void init();
	void mainLoop();
	void cleanup();
public:
	//Проверяем наличие дебага
	#ifdef NDEBUG
		const bool enableValidationLayers = false;
	#else
		const bool enableValidationLayers = true;
	#endif

	void run();
	//Опрашиваем заинтересованные объекты что бы собрать подключаемые расширения
	std::vector<const char*> getRequiredExtensions();
};