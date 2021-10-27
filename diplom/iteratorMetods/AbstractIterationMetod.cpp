#include "stdafx.h"

#include "AbstractIterationMetod.h"

void AbstractItertionMetod::changePosition(glm::vec3 newPosition)
{
	currentPosition = newPosition;
}

void AbstractItertionMetod::setDelta(float newDelta)
{
	delta = newDelta;
}

void AbstractItertionMetod::setTime(float newTime)
{
	time = newTime;
}
