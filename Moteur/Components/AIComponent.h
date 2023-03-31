#pragma once

#include "ECS/Component.h"
#include "TransformComponent.h"

enum BehaviorType
{
	SEEK,
	FLEE,
	PURSUE
};

struct AIComponent : Component
{
public:
	AIComponent() : ComponentType("AIComponent"), transformComponent(nullptr) {}

	// Trigger radius between player and AI agent
	float radius;

	// The speed
	float speed;

	// The behavior type
	BehaviorType type;

	// The agent's transformation component.
	TransformComponent* transformComponent;

	// The destructor
	~AIComponent() {};

private:
	// The component type
	std::string ComponentType;
};
