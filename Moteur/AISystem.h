#pragma once

#include <glm/glm.hpp>
#include <ECS/System.h>
#include "AIComponent.h"

class AISystem : public System
{
public:
	// Constructor
	AISystem();

	// The update method called every tick
	void Process(const std::vector<Entity*>& entities, float dt);

	// Pursue Steering behaviour
	void Pursue(float dt, float distance, glm::vec3 playerPosition, AIComponent* aiComponent);

	// Gracefully closes everything down
	void Shutdown();

	// Destructor
	~AISystem();

private:
	// Enemy face the direction of player
	void Face(glm::vec3 targetPosition, AIComponent* aiComponent);

	// Get the distance between player and enemy
	float GetDistance(glm::vec3 aiPosition, glm::vec3 playerPosition);

	// Get the player component
	TransformComponent* GetPlayerComponent(const std::vector<Entity*>& entities);
};
