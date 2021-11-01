#pragma once
#include "stdafx.h"
#include "iteratorMetods/RungeKutte3d.h"
#include "WindowManager.h"
#include "Render.h"
#include "Camera.h"

class DiplomApp
{

private:
	RungeKutte3d rk3d;
	WindowManager whindow;
	Render render;
	Camera camera;

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