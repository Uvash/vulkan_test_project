#pragma once
#include "stdafx.h"

//#include "DiplomApp.h"
class DiplomApp;

class WindowManager
{
private:
	//Указатель на наше приложение
	DiplomApp *app;
	//Размеры окна при создании
	const uint32_t WIDTH = 800;
	const uint32_t HEIGHT = 600;
	//Указатель на создаваемое окно
	GLFWwindow * window = nullptr;
	//Флаг сигнализирующий об изменениях размера окна
	bool framebufferResized = true;
	//Флаг сигнализирующий об инициализации окна
	bool windowCreated = false;
public:
	WindowManager();
	~WindowManager();
	//Отложенная инициализация окна
	void windowManagerInit(DiplomApp* new_app);
	//вызываем в случаи изменения размеров окна
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	//опрашиваем окно о необходимости закрытия
	bool shoudClose();
	//опрашиваем окно о событиях
	void pollEvents();
	//сообщает о необходимых расширениях окна
	std::vector<const char*> getRequiredExtensions();
	//связывает поверхность с окном
	void createSurface(VkInstance& instance, VkSurfaceKHR& surface);
	//Узнаёт текущие размеры окон
	void getFramebufferSize(int &width, int &height);
};