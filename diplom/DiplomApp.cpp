#include "stdafx.h"
#include "DiplomApp.h"

void DiplomApp::init()
{
	rk3d.changePosition({ 10, 10, 10 });
	rk3d.setDelta(0.005);
	camera.init();
	camera.setSpeed(10.0f);
	whindow.windowManagerInit(this, &camera);
	render.RenderInit(this, &whindow, &rk3d, &camera);
}

void DiplomApp::mainLoop()
{
	while (!whindow.shoudClose())
	{
		whindow.pollEvents();
		camera.updateView();
		render.drawFrame();
		rk3d.step();
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
