#pragma once

#include <ECS/Component.h>
#include <glm/vec3.hpp>

// Component holding info about object motion
struct VelocityCompoent : Component {
public:
	VelocityCompoent() :
		componentType("VelocityCompoent"),
		velocity(glm::vec3(0.f)),
		destination(glm::vec3(0.f)),
		targeting(false)
	{}
	~VelocityCompoent() {}

	glm::vec3 velocity;
	glm::vec3 destination;

	bool targeting;

private:
	std::string componentType;
};