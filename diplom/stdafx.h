#pragma once
/*
Набор предкомпилируемых заголовочных файлов
Класть только то, что редко изменяется
*/
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vulkan/vulkan.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <cstdint> // Necessary for UINT32_MAX
#include <fstream>
#include <memory>
#include <optional>
#include <set>
