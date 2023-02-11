#pragma once

#include <ECS/Component.h>

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

struct TransformComponent : Component {
public:
	TransformComponent() :
		componentType("TransfromComponent"),
		position(glm::vec3(0.f)), 
		scale(glm::vec3(0.f)), 
		rotation(glm::quat(glm::vec3(0.f))) {}

	~TransformComponent() {}

private:
	std::string componentType;

	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;
};