#include "stdafx.h"
#include "DiplomApp.h"

void DiplomApp::init()
{
	rk2d.changePosition({ 1, 0, 0 });
	rk2d.setH(0.01);
	whindow.windowManagerInit(this);
	render.RenderInit(this, &whindow);
}

void DiplomApp::mainLoop()
{
	
	while (!whindow.shoudClose())
	{
		whindow.pollEvents();
		render.drawFrame();
		rk2d.step();
	}
	render.idleDevice();
}
void DiplomApp::cleanup()
{

}

void DiplomApp::run()
{
	init();
	mainLoop();
	cleanup();
}

std::vector<const char*> DiplomApp::getRequiredExtensions()
{
	
	std::vector<const char*> extensions;
	//Спрашиваем окно о необходимых ему расширениях
	extensions = whindow.getRequiredExtensions();
	//Проверяем флаг дебага 
	//Если мы в дебаге, докидываем ещё одно расширение для отладки
	if (enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}
