#include "stdafx.h"

#include "RungeKutte2d.h"

float RungeKutte2d::f(glm::vec3 currentPoint)
{
	return 3.0f * currentPoint.x + 4.0f * currentPoint.y;
}

float RungeKutte2d::g(glm::vec3 currentPoint)
{
	return 2.0f * currentPoint.x + currentPoint.y;
}
glm::vec3 RungeKutte2d::calculateNewValue()
{
	//Вычисляем новое значение
	glm::vec2 k1;
	k1.x = h * f(currentPosition);
	k1.y = h * g(currentPosition);

	glm::vec2 k2;
	k2.x = h * f({ currentPosition.x + k1.x / 2.0f, currentPosition.y + k1.y / 2.0f, currentPosition.z + h / 2.0f });
	k2.y = h * g({ currentPosition.x + k1.x / 2.0f, currentPosition.y + k1.y / 2.0f, currentPosition.z + h / 2.0f });

	glm::vec2 k3;
	k3.x = h * f({ currentPosition.x + k2.x / 2.0f, currentPosition.y + k2.y / 2.0f, currentPosition.z + h / 2.0f });
	k3.y = h * g({ currentPosition.x + k2.x / 2.0f, currentPosition.y + k2.y / 2.0f, currentPosition.z + h / 2.0f });

	glm::vec2 k4;
	k4.x = h * f({ currentPosition.x + k3.x, currentPosition.y + k3.y, currentPosition.z + h });
	k4.y = h * g({ currentPosition.x + k3.x, currentPosition.y + k3.y, currentPosition.z + h });

	glm::vec3 result;
	result.x = currentPosition.x + (k1.x + k2.x * 2.0f + k3.x * 2.0f + k4.x) / 6.0f;
	result.y = currentPosition.y + (k1.y + k2.y * 2.0f + k3.y * 2.0f + k4.y) / 6.0f;
	result.z = currentPosition.z + h;

	return result;
}
void RungeKutte2d::step()
{
	if (result.size() > 1)
	{
		glm::vec3 last = result.back();
		if (last.x > 10 || last.y > 10)
			return;
	}
	glm::vec3 newValue = calculateNewValue();
	currentPosition = newValue;
	result.push_back(newValue);

	std::cout << "x: " << newValue.x << " y: " << newValue.y << " z: " << newValue.z << std::endl;
}

void RungeKutte2d::changePosition(glm::vec3 newPosition)
{
	currentPosition = newPosition;
}

void RungeKutte2d::setH(float newH)
{
	h = newH;
}