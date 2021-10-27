#include "stdafx.h"
#include "DiplomApp.h"

void DiplomApp::init()
{
	rk3d.changePosition({ 10, 10, 10 });
	rk3d.setDelta(0.005);
	whindow.windowManagerInit(this);
	render.RenderInit(this, &whindow, &rk3d);
}

void DiplomApp::mainLoop()
{
	
	while (!whindow.shoudClose())
	{
		whindow.pollEvents();
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
	//���������� ���� � ����������� ��� �����������
	extensions = whindow.getRequiredExtensions();
	//��������� ���� ������ 
	//���� �� � ������, ���������� ��� ���� ���������� ��� �������
	if (enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}
