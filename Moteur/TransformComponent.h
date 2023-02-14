#pragma once

#include <ECS/Component.h>

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

// Struct holding the transform information of a mesh
struct TransformComponent : Component {
public:
	// Constructor
	TransformComponent() :
		componentType("TransfromComponent"),
		position(glm::vec3(0.f)), 
		scale(glm::vec3(0.f)), 
		rotation(glm::quat(glm::vec3(0.f))) 
	{}

	// Destructor
	~TransformComponent() {}

	// Mesh position
	glm::vec3 position;
	// Mesh size
	glm::vec3 scale;
	// Mesh rotation
	glm::quat rotation;

private:

	// name of the component
	std::string componentType;
};