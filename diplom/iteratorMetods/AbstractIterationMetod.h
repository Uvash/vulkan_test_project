#pragma once
#include "stdafx.h"

class AbstractItertionMetod
{
public:
	std::vector<glm::vec3> result;
	//Функция делающая шаг метода
	virtual void step() = 0;
	void changePosition(glm::vec3 newPosition);
	void setTime(float newTime);
	void setDelta(float delta);
protected:
	float delta;
	float time;
	glm::vec3 currentPosition;
	//Функция определяемая движение точки 
	virtual float f(glm::vec3 currentPoint, float currentTime) = 0; // по оси х
	virtual float g(glm::vec3 currentPoint, float currentTime) = 0; // по оси y
	virtual float h(glm::vec3 currentPoint, float currentTime) = 0; // по оси z

	virtual glm::vec3 calculateNewValue() = 0;
};