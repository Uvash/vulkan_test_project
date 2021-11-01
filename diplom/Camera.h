#pragma once
#include "stdafx.h"
#include "HelpStructures.h"

class Camera
{
public:
	glm::vec3 inputAxis;
	UniformBufferObject* getUniformBuffer();
	void updateView();
	void setProj(float newAspect = (4.0f / 3.0f), float newFovy = 90.0f, float newNear = 0.1f, float newFar = 100.0f);
	void setSpeed(float newSpeed);
	void init();
private:
	UniformBufferObject uniformBuffer;
	float speed;
	VkExtent2D* swapChainExtent;

	//Вектора для view
	glm::vec3 eye;
	glm::vec3 center;
	glm::vec3 up;

	//переменные для Proj
	float fovy;
	float aspect;
	float near;
	float far;
	//Время прошедшее с превидущего обновления
	std::chrono::steady_clock::time_point lastTime;
};