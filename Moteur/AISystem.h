#pragma once

#include <glm/glm.hpp>
#include <ECS/System.h>
#include "AIComponent.h"

class AISystem : public System
{
public:
	// Constructor
	AISystem();
	void Process(const std::vector<Entity*>& entities, float dt);
	void Pursue(float dt, float distance);
	void Shutdown();

	// Destructor
	~AISystem();

private:
	AIComponent* component;
	void Face();
	float GetDistance();
};
