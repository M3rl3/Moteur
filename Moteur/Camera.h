#pragma once

#include <glm/vec3.hpp>

struct Camera {
	Camera() : position(glm::vec3(0.f)), target(glm::vec3(0.f)) {}

	glm::vec3 position;
	glm::vec3 target;
};