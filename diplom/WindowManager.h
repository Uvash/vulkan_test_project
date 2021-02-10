#pragma once
#include "stdafx.h"

//#include "DiplomApp.h"
class DiplomApp;

class WindowManager
{
private:
	//��������� �� ���� ����������
	DiplomApp *app;
	//������� ���� ��� ��������
	const uint32_t WIDTH = 800;
	const uint32_t HEIGHT = 600;
	//��������� �� ����������� ����
	GLFWwindow * window = nullptr;
	//���� ��������������� �� ���������� ������� ����
	bool framebufferResized = true;
	//���� ��������������� �� ������������� ����
	bool windowCreated = false;
public:
	WindowManager();
	~WindowManager();
	//���������� ������������� ����
	void windowManagerInit(DiplomApp* new_app);
	//�������� � ������ ��������� �������� ����
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	//���������� ���� � ������������� ��������
	bool shoudClose();
	//���������� ���� � ��������
	void pollEvents();
	std::vector<const char*> getRequiredExtensions();
};