#pragma once

#include <glm/vec3.hpp>

// Simple struct holding camera position and lookAt
struct Camera {
	// Constructor
	Camera() : 
		position(glm::vec3(0.f)), 
		target(glm::vec3(0.f)) 
	{}
	// Destructor
	~Camera() {}

	// Camera position
	glm::vec3 position;

	// Camera lookAt
	glm::vec3 target;
};