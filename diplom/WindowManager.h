#pragma once
#include "stdafx.h"

class WindowManager
{
private:
	//–азмеры окна при создании
	const uint32_t WIDTH = 800;
	const uint32_t HEIGHT = 600;
	//”казатель на создаваемое окно
	GLFWwindow* window = nullptr;
	//‘лаг сигнализирующий об изменени€х размера окна
	bool framebufferResized = true;
	//‘лаг сигнализирующий об инициализации окна
	bool windowCreated = false;
public:
	WindowManager();
	~WindowManager();
	//ќтложенна€ инициализаци€ окна
	void windowManagerInit();
	//вызываем в случаи изменени€ размеров окна
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	//опрашиваем окно о необходимости закрыти€
	bool shoudClose();
	//опрашиваем окно о событи€х
	void pollEvents();
};