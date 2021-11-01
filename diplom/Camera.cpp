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
	//вычисляем скорость
	eye += (deltaTime * speed) * inputAxis;
	center += (deltaTime * speed) * inputAxis;
	uniformBuffer.view = glm::lookAt(eye, center, up);
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

	eye = { 30.0f, 30.0f, 40.0f };
	center = { 0.0f, 0.0f, 0.0f };
	up = { 0.0f, 0.0f, 1.0f };
	uniformBuffer.view = glm::lookAt(eye, center, up);

	setProj();

}