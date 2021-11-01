#pragma once
#include "stdafx.h"

//#include "DiplomApp.h"
class DiplomApp;
class Camera;

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

	Camera* camera;
public:
	WindowManager();
	~WindowManager();
	//Отложенная инициализация окна
	void windowManagerInit(DiplomApp* new_app, Camera* newCamera);
	//вызываем в случаи изменения размеров окна
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	//вызываем статическую обёртку в случаи нажатия на кнопку
	static void static_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void key_callback(int key, int scancode, int action, int mods);
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
	//Устанавливает флаг framebufferResized
	void setFramebufferResized(bool newFlag);
};