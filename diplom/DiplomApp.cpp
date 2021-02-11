#include "stdafx.h"
#include "DiplomApp.h"

void DiplomApp::init()
{
	whindow.windowManagerInit(this);
	render.RenderInit(this, &whindow);
}

void DiplomApp::mainLoop()
{
	
	while (!whindow.shoudClose())
	{
		whindow.pollEvents();
	}
	
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
