#include "AISystem.h"

AISystem::AISystem()
{
}

void AISystem::Process(const std::vector<Entity*>& entities, float dt)
{
	AIComponent* aiComponent = nullptr;
	TransformComponent* transformComponent = nullptr;

	for (Entity* entity : entities) {

		aiComponent = entity->GetComponentByType<AIComponent>();

		if (aiComponent != nullptr) {

			// find player component
			transformComponent = GetPlayerComponent(entities);

			if (transformComponent != nullptr) {

				glm::vec3 aiPosition = aiComponent->transformComponent->position;
				glm::vec3 playerPosition = transformComponent->position;

				float distance = GetDistance(aiPosition, playerPosition);

				if (distance < aiComponent->radius) {

					switch (aiComponent->type)
					{
					case BehaviorType::PURSUE:
						Pursue(dt, distance, playerPosition, aiComponent);
						break;
					case BehaviorType::SEEK:
						break;
					case BehaviorType::FLEE:
						break;
					default:
						break;
					}
				}
			}
		}
	}
}

void AISystem::Pursue(float dt, float distance, glm::vec3 playerPosition, AIComponent* aiComponent)
{
	Face(playerPosition, aiComponent);
	float predictTime = distance / aiComponent->speed;
	glm::vec3 predictedTargetPosition = playerPosition * predictTime;
	glm::vec3 position = aiComponent->transformComponent->position;
	glm::vec3 desiredVelocity = glm::normalize(predictedTargetPosition - position) * aiComponent->speed;

	aiComponent->transformComponent->position += desiredVelocity;
}

void AISystem::Shutdown()
{
	return;
}

AISystem::~AISystem()
{
}

void AISystem::Face(glm::vec3 targetPosition, AIComponent* aiComponent)
{
	glm::vec3 currentPosition = aiComponent->transformComponent->position;

	glm::vec3 direction = glm::normalize(targetPosition - currentPosition);
	float angle = glm::degrees(glm::atan(direction.x, direction.z));

	aiComponent->transformComponent->rotation = glm::quat(glm::vec3(0.f, angle, 0.f));
}

float AISystem::GetDistance(glm::vec3 aiPosition, glm::vec3 playerPosition)
{
	return glm::distance(aiPosition, playerPosition);
}

TransformComponent* AISystem::GetPlayerComponent(const std::vector<Entity*>& entities)
{
	TransformComponent* component = nullptr;

	for (Entity* entity : entities) {
		component = entity->GetComponentByType<TransformComponent>();
		if (component->GetType() == "Player") {
			return component;
		}
	}
	return component;
}
