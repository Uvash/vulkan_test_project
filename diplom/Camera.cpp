#include "stdafx.h"
#include "Camera.h"

UniformBufferObject* Camera::getUniformBuffer()
{
	return &uniformBuffer;
}
void Camera::updateView()
{
	std::chrono::steady_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
	lastTime = currentTime;
	//вычисляем поворот камеры
	glm::vec3 direction;
	direction.x = cos(glm::radians(rotationAxis.x)) * cos(glm::radians(rotationAxis.y));
	direction.y = sin(glm::radians(rotationAxis.x)) * cos(glm::radians(rotationAxis.y));
	direction.z = sin(glm::radians(rotationAxis.y));
	cameraFront = glm::normalize(direction);

	//вычисляем cмещение камеры относительно угла поворота
	if (inputAxis.x != 0)
	{
		glm::vec3 front = cameraFront ;
		eye += front * (deltaTime * speed) * inputAxis.x;
	}
	if (inputAxis.y != 0)
	{	//получаем вектор
		glm::vec3 bottom = glm::normalize(glm::cross(cameraFront, up));
		eye += bottom * (deltaTime * speed * inputAxis.y);
	}
	if (inputAxis.z != 0)
	{
		glm::vec3 right = glm::normalize(glm::cross(cameraFront, up));
		//умножаем на -1 поскольку вектор перевёрнут
		glm::vec3 top = -1.0f * glm::normalize(glm::cross(cameraFront, right));
		eye += top * (deltaTime * speed * inputAxis.z);
	}

	//Блокируем тангаж -90 до 90 что бы не было переворотов
	if (rotationAxis.y > 89.0)
		rotationAxis.y = 89.0;
	if (rotationAxis.y < -89.0)
		rotationAxis.y = -89.0;

	uniformBuffer.view = glm::lookAt(eye, eye + cameraFront, up);
}
void Camera::setSpeed(float newSpeed)
{
	speed = newSpeed;
}

void Camera::setProj(float newAspect, float newFovy, float newNear, float newFar)
{
	uniformBuffer.proj = glm::perspective(glm::radians(newFovy), newAspect, newNear, newFar);
	uniformBuffer.proj[1][1] *= -1;
}

void Camera::init()
{

	uniformBuffer.model = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));

	eye = { 0.0f, 0.0f, 0.0f };
	cameraFront = { 1.0f, 1.0f, 0.0f };
	up = { 0.0f, 0.0f, 1.0f };
	uniformBuffer.view = glm::lookAt(eye, cameraFront, up);

	setProj();

}