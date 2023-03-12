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
	void Pursue(float dt, float distance, glm::vec3 playerPosition, AIComponent* aiComponent);
	void Shutdown();

	// Destructor
	~AISystem();

private:
	void Face(glm::vec3 targetPosition, AIComponent* aiComponent);
	float GetDistance(glm::vec3 aiPosition, glm::vec3 playerPosition);
	TransformComponent* GetPlayerComponent(const std::vector<Entity*>& entities);
};
