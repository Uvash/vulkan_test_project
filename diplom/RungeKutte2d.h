#pragma once


#include "stdafx.h"


class RungeKutte2d
{
private:
	float h;
	//z - ����� ������� �����
	glm::vec3 currentPosition;
	//������� ������������ �������� ����� �� ��� �
	float f(glm::vec3 currentPoint);
	//������� ������������ �������� ����� �� ��� �
	float g(glm::vec3 currentPoint);
	glm::vec3 calculateNewValue();
public:
	std::vector<glm::vec3> result;
	//������� �������� ��� ������
	void step();
	void changePosition(glm::vec3 newPosition);
	void setH(float newH);
};