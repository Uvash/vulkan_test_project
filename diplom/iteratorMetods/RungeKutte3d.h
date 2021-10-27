#pragma once
#include "../stdafx.h"
#include "AbstractIterationMetod.h"

class RungeKutte3d : public AbstractItertionMetod
{
public:
	//������� �������� ��� ������
	virtual void step();
	
protected:
	//������� ������������ �������� ����� 
	virtual float f(glm::vec3 currentPoint, float currentTime); // �� ��� �
	virtual float g(glm::vec3 currentPoint, float currentTime); // �� ��� y
	virtual float h(glm::vec3 currentPoint, float currentTime); // �� ��� z
	virtual glm::vec3 calculateNewValue();
};