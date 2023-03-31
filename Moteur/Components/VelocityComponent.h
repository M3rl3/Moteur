#pragma once

#include <ECS/Component.h>
#include <glm/vec3.hpp>

// Component holding info about object motion
struct VelocityComponent : Component {
public:
	VelocityComponent() :
		componentType("VelocityComponent"),
		velocity(glm::vec3(0.f)),
		destination(glm::vec3(0.f)),
		targeting(false),
		useVelocity(true)
	{}
	~VelocityComponent() {}

	glm::vec3 velocity;
	glm::vec3 destination;

	bool targeting;
	bool useVelocity;

private:
	std::string componentType;
};