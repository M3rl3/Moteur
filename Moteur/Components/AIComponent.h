#pragma once

#include "ECS/Component.h"

#include <glm/vec3.hpp>

enum BehaviourType
{
	IDLE,
	SEEK,
	FLEE,
	PURSUE,
	EVADE,
	APPROACH,
	CATCH
};

struct AIComponent : Component
{
public:
	AIComponent() :
		ComponentType("AIComponent"),
		aiBehaviour(BehaviourType::SEEK),
		speed(0.f),
		radius(0.f),
		aiVelocity(glm::vec3(0))
	{};

	// The destructor
	~AIComponent() {};

	// Trigger radius between player and AI agent
	float radius;

	// Movement speed
	float speed;

	// a temp velocity
	glm::vec3 aiVelocity;

	// The behavior type
	BehaviourType aiBehaviour;

private:
	// The component type
	std::string ComponentType;
};
