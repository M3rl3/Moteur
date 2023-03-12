#include "AISystem.h"

AISystem::AISystem()
{
}

void AISystem::Process(const std::vector<Entity*>& entities, float dt)
{
	for (Entity* entity : entities) {
		component = entity->GetComponentByType<AIComponent>();
		if ((component != nullptr) && (GetDistance() < component->radius)) {
			switch (component->type)
			{
			case BehaviorType::PURSUE:
				Pursue(dt, GetDistance());
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

void AISystem::Pursue(float dt, float distance)
{
	Face();
	float predictTime = distance / component->speed;
	glm::vec3 predictedTargetPosition = component->targetPosition * predictTime;
	glm::vec3 position = component->transformComponent->position;
	glm::vec3 desiredVelocity = glm::normalize(predictedTargetPosition - position) * component->speed;

	component->transformComponent->position += desiredVelocity;
}

void AISystem::Shutdown()
{
	return;
}

AISystem::~AISystem()
{
}

void AISystem::Face()
{
	glm::vec3 targetPosition = component->targetPosition;
	glm::vec3 currentPosition = component->transformComponent->position;

	glm::vec3 direction = glm::normalize(targetPosition - currentPosition);
	float angle = glm::degrees(glm::atan(direction.x, direction.z));

	component->transformComponent->rotation = glm::quat(glm::vec3(0.f, angle, 0.f));
}

float AISystem::GetDistance()
{
	return glm::distance(component->targetPosition, component->transformComponent->position);
}
