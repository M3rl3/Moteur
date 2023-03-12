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

	float radius;
	float speed;
	BehaviorType type;
	TransformComponent* transformComponent;

	~AIComponent() {};

private:
	std::string ComponentType;
};
