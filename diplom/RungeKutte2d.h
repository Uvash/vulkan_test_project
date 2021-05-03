#pragma once


#include "stdafx.h"


class RungeKutte2d
{
private:
	float h;
	//z - будет хранить время
	glm::vec3 currentPosition;
	//Функция определяемая движение точки по оси х
	float f(glm::vec3 currentPoint);
	//Функция определяемая движение точки по оси у
	float g(glm::vec3 currentPoint);
	glm::vec3 calculateNewValue();
public:
	std::vector<glm::vec3> result;
	//Функция делающая шаг метода
	void step();
	void changePosition(glm::vec3 newPosition);
	void setH(float newH);
};