#include "stdafx.h"

#include "RungeKutte3d.h"

void RungeKutte3d::step()
{
	if (result.size() > 10)
	{
		return;
	}
	if (result.size() > 1 )
	{
		glm::vec3 last = result.back();
		if (last.x > 100 || last.y > 100)
			return;
	}
	glm::vec3 newValue = calculateNewValue();
	currentPosition = newValue;
	result.push_back(newValue);

	std::cout << "x: " << newValue.x << " y: " << newValue.y << " z: " << newValue.z << " time: " << time << std::endl;
}

float RungeKutte3d::f(glm::vec3 currentPoint, float currentTime)
{
	return 10.0f * (currentPoint.y - currentPoint.x);
}

float RungeKutte3d::g(glm::vec3 currentPoint, float currentTime)
{
	return currentPoint.x * (28.0 - currentPoint.z) - currentPoint.y;
}

float RungeKutte3d::h(glm::vec3 currentPoint, float currentTime)
{
	return currentPoint.x * currentPoint.y - (8.0f / 3.0f) * currentPoint.z;
}

glm::vec3 RungeKutte3d::calculateNewValue()
{
	//Вычисляем новое значение
	glm::vec3 k1;
	k1.x = delta * f(currentPosition, time);
	k1.y = delta * g(currentPosition, time);
	k1.z = delta * h(currentPosition, time);

	glm::vec3 k2;
	k2.x = delta * f({ currentPosition.x + k1.x / 2.0f, currentPosition.y + k1.y / 2.0f, currentPosition.z + k1.z / 2.0f }, time + delta / 2.0f);
	k2.y = delta * g({ currentPosition.x + k1.x / 2.0f, currentPosition.y + k1.y / 2.0f, currentPosition.z + k1.z / 2.0f }, time + delta / 2.0f);
	k2.z = delta * h({ currentPosition.x + k1.x / 2.0f, currentPosition.y + k1.y / 2.0f, currentPosition.z + k1.z / 2.0f }, time + delta / 2.0f);

	glm::vec3 k3;
	k3.x = delta * f({ currentPosition.x + k2.x / 2.0f, currentPosition.y + k2.y / 2.0f, currentPosition.z + k2.z / 2.0f }, time + delta / 2.0f);
	k3.y = delta * g({ currentPosition.x + k2.x / 2.0f, currentPosition.y + k2.y / 2.0f, currentPosition.z + k2.z / 2.0f }, time + delta / 2.0f);
	k3.z = delta * h({ currentPosition.x + k2.x / 2.0f, currentPosition.y + k2.y / 2.0f, currentPosition.z + k2.z / 2.0f }, time + delta / 2.0f);

	glm::vec3 k4;
	k4.x = delta * f({ currentPosition.x + k3.x, currentPosition.y + k3.y, currentPosition.z + k3.z }, time + delta);
	k4.y = delta * g({ currentPosition.x + k3.x, currentPosition.y + k3.y, currentPosition.z + k3.z }, time + delta);
	k4.z = delta * h({ currentPosition.x + k3.x, currentPosition.y + k3.y, currentPosition.z + k3.z }, time + delta);


	glm::vec3 result;
	result.x = currentPosition.x + (k1.x + k2.x * 2.0f + k3.x * 2.0f + k4.x) / 6.0f;
	result.y = currentPosition.y + (k1.y + k2.y * 2.0f + k3.y * 2.0f + k4.y) / 6.0f;
	result.z = currentPosition.z + (k1.z + k2.z * 2.0f + k3.z * 2.0f + k4.z) / 6.0f;

	return result;
}