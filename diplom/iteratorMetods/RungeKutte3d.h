#pragma once
#include "../stdafx.h"
#include "AbstractIterationMetod.h"

class RungeKutte3d : public AbstractItertionMetod
{
public:
	//Функция делающая шаг метода
	virtual void step();
	
protected:
	//Функция определяемая движение точки 
	virtual float f(glm::vec3 currentPoint, float currentTime); // по оси х
	virtual float g(glm::vec3 currentPoint, float currentTime); // по оси y
	virtual float h(glm::vec3 currentPoint, float currentTime); // по оси z
	virtual glm::vec3 calculateNewValue();
};