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
	//��������� ������� ������
	#ifdef NDEBUG
		const bool enableValidationLayers = false;
	#else
		const bool enableValidationLayers = true;
	#endif

	void run();
	//���������� ���������������� ������� ��� �� ������� ������������ ����������
	std::vector<const char*> getRequiredExtensions();
};