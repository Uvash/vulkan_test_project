#pragma once
#include "stdafx.h"

class WindowManager
{
private:
	//������� ���� ��� ��������
	const uint32_t WIDTH = 800;
	const uint32_t HEIGHT = 600;
	//��������� �� ����������� ����
	GLFWwindow* window = nullptr;
	//���� ��������������� �� ���������� ������� ����
	bool framebufferResized = true;
	//���� ��������������� �� ������������� ����
	bool windowCreated = false;
public:
	WindowManager();
	~WindowManager();
	//���������� ������������� ����
	void windowManagerInit();
	//�������� � ������ ��������� �������� ����
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	//���������� ���� � ������������� ��������
	bool shoudClose();
	//���������� ���� � ��������
	void pollEvents();
};