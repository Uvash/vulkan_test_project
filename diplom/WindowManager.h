#pragma once
#include "stdafx.h"

//#include "DiplomApp.h"
class DiplomApp;
class Camera;

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

	Camera* camera;
public:
	WindowManager();
	~WindowManager();
	//���������� ������������� ����
	void windowManagerInit(DiplomApp* new_app, Camera* newCamera);
	//�������� � ������ ��������� �������� ����
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	//�������� ����������� ������ � ������ ������� �� ������ �� ����������
	static void static_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	//��������� ����� �� ������������ ������
	void key_callback(int key, int scancode, int action, int mods);
	//�������� ����������� ������ � ������ ������� �� ������ �� �����
	static void static_button_callback(GLFWwindow* window, int button, int action, int mods);
	void button_callback(int button, int action, int mods);
	//������ ���������� � ������ �������� ����
	static void static_cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	void cursor_position_callback(double xpos, double ypos);
	bool buttonPressed = false;
	glm::dvec2 lastPos;
	//���������� ���� � ������������� ��������
	bool shoudClose();
	//���������� ���� � ��������
	void pollEvents();
	//�������� � ����������� ����������� ����
	std::vector<const char*> getRequiredExtensions();
	//��������� ����������� � �����
	void createSurface(VkInstance& instance, VkSurfaceKHR& surface);
	//����� ������� ������� ����
	void getFramebufferSize(int &width, int &height);
	//������������� ���� framebufferResized
	void setFramebufferResized(bool newFlag);
};