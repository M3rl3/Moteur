#pragma once

#include <glm/glm.hpp>
#include <ECS/System.h>

#include "../Components/AIComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/MeshComponent.h"

class AISystem : public System
{
public:
	// Constructor
	AISystem();

	// The update method called every tick
	void Process(const std::vector<Entity*>& entities, float dt);

	// Pursue Steering behaviour
	void Pursue(float dt, float distance, glm::vec3 playerPosition, AIComponent* aiComponent, TransformComponent* transformComponent);

	// Gracefully closes everything down
	void Shutdown();

	// Destructor
	~AISystem();

private:
	// Enemy face the direction of player
	void Face(glm::vec3 targetPosition, TransformComponent* transformComponent);

	// Get the distance between player and enemy
	float GetDistance(glm::vec3 aiPosition, glm::vec3 playerPosition);

	// Get the player components
	TransformComponent* GetPlayerTransform(const std::vector<Entity*>& entities);
	VelocityComponent* GetPlayerVelocity(const std::vector<Entity*>& entities);
	MeshComponent* GetPlayerMesh(const std::vector<Entity*>& entities);
};
